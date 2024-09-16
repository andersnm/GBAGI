#include <string>
#include <vector>
#include <tuple>
#include "commands.h"
#include "romlib.h"
#include "words_parser.h"
#include "agi_parser.h"
#include "gbagi_writer.h"

int main() {
    std::vector<std::tuple<std::string, std::string>> inputs = {
        std::make_tuple(".\\kq3_mac\\", "KQ3 (MAC)"),
        std::make_tuple(".\\kq3_amiga\\", "KQ3 (Amiga)"),
        std::make_tuple(".\\kq1_dos\\", "KQ1 (DOS)"),
        std::make_tuple(".\\kq1_mac\\", "KQ1 (MAC)"),
        std::make_tuple(".\\lsl_amiga\\", "LSL (Amiga)"),
        std::make_tuple(".\\mh1_dos\\", "MH1 (DOS)"),
        std::make_tuple(".\\mh2_amiga\\", "MH2 (Amiga)"),
        std::make_tuple(".\\mh1_st\\", "Manhunter (ST)"),
        std::make_tuple(".\\mh2_st\\", "Manhunter 2 (ST)"),
        std::make_tuple(".\\sq1_dos\\", "SQ1 (DOS)"),
        std::make_tuple(".\\sq1_amiga\\", "SQ1 (Amiga)"), // Logic 99 is odd
        std::make_tuple(".\\sq2_amiga\\", "SQ2 (Amiga)"),
    };

    std::vector<agi_parser> games;
    for (const std::tuple<std::string, std::string>& input : inputs) {
        agi_parser vp;
        if (!vp.load_game(std::get<0>(input), std::get<1>(input))) {
            printf("Cannot load game ");
            printf(std::get<0>(input).c_str());
            return 3;
        }

        games.push_back(vp);
    }

    std::vector<word_t> vocab_words;
    words_parser vwp(vocab_words);
    if (!vwp.load_vocab("c:\\Users\\andyw\\Code\\GBAGI\\romgui\\vocab.bin")) {
        printf("Cannot open vocab.bin");
        return 2;
    }

    words_util vocab_bin(vocab_words);

    std::vector<uint8_t> binary;
    if (!read_binary("c:\\Users\\andyw\\Code\\GBAGI\\gbagi.bin", binary)) {
        printf("Cannot open gbagi.bin");
        return 2;
    }

    std::string filename = "out.gba";
    FILE* fout;
    if ((fout = fopen(filename.c_str(), "wb")) == NULL) {
        printf("Cannot open %s\n", filename.c_str());
        return 1;
    }

    // Copy gbagi.bin straight, then align before output_games

#define AGI_DATA_ALIGNMENT 0x400
#define BASE800	0x08000000

    uint32_t BASEx0X = (binary.size() + AGI_DATA_ALIGNMENT - 1) & -AGI_DATA_ALIGNMENT;
    uint32_t BASE80X = BASE800 + BASEx0X;

    gbagi_writer writer(fout, BASE80X);
    writer.write_buf(binary);

    for (size_t i = binary.size(); i < BASEx0X; i++)
        writer.write_u8(0xFF);

    writer.output_games(games, vocab_bin);

    fclose(fout);

	return 0;
}