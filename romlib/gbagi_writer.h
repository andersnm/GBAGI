#pragma once
#include <algorithm>
#include <array>
#include "romlib.h"
#include "agi_parser.h"

#undef min
#undef max

struct gbagi_writer {
    FILE* fout;
    long base;
    long offs;

    gbagi_writer(FILE* f, long base) {
        this->fout = f;
        this->base = base;
        this->offs = 0;
    }

    void write_u8(uint8_t b) {
        fwrite(&b, 1, 1, fout);
        // fputc(b, fout);
        this->offs++;
    }

    void write_u16(uint16_t l) {
        write_u8(l & 0xFF);
        write_u8((l >> 8) & 0xFF);
    }

    void write_buf(const std::vector<uint8_t>& b) {
        if (b.empty()) {
            return;
        }

        fwrite(&b[0], b.size(), 1, fout);
        this->offs += b.size();
    }

    void write_u32(uint32_t l) {
        write_u8(l & 0xFF);
        write_u8((l >> 8) & 0xFF);
        write_u8((l >> 16) & 0xFF);
        write_u8((l >> 24) & 0xFF);
    }

    template <std::size_t size>
    void write_u32_array(std::array<uint32_t, size> arr) {
        for (size_t i = 0; i < size; i++) {
            write_u32(arr[i]);
        }
    }

    template <std::size_t size>
    void write_u8_array(std::array<uint8_t, size> arr) {
        for (size_t i = 0; i < size; i++) {
            write_u8(arr[i]);
        }
    }

    void write_sz(const std::string& s) {
        fwrite(s.c_str(), s.length() + 1, 1, fout);
        offs += s.length() + 1;
    }

    void write_voldata(const std::array<dir_info_t, 4>& dirs, size_t volnum, int offset, std::array<std::array<uint32_t, 256>, 4>& dir_offsets) {
        for (size_t dirnum = 0; dirnum < 4; dirnum++) {
            const auto& dir = dirs[dirnum];
            for (size_t entnum = 0; entnum < dir.ents.size(); entnum++) {
                const dir_ent_t& ent = dir.ents[entnum];
                if (ent.vol == volnum) {
                    // write 5 byte header and unpacked buffer
                    dir_offsets[dirnum][entnum] = this->base + offset;

                    write_u16(0x1234);
                    write_u8(ent.vi);
                    uint16_t declen = (uint16_t)ent.decbuffer.size();
                    write_u16(declen);
                    write_buf(ent.decbuffer);
                    offset += 5 + declen;
                }
            }
        }
    }

    void write_inventory_items(const agi_parser& vp, std::array<uint32_t, 256>& object_offsets, std::array<uint8_t, 256>& object_rooms) {

        int object_offset = this->base + this->offs;

        for (size_t i = 0; i < vp.objects.size(); i++) {
            const auto& obj = vp.objects[i];

            object_offsets[i] = object_offset;
            object_rooms[i] = obj.room;

            write_sz(obj.name);

            object_offset += obj.name.length() + 1;
        }

        // Original calculated and copied "entsize" extra bytes at the end from outside the source buffer (NOT NEEDED)
        for (size_t i = 0; i < 3; i++) {
            write_u8(0);
        }
    }

    void write_word_data(const agi_parser& vp, const words_util& vocab_bin, std::array<uint32_t, 26>& word_offsets, int& max_groups) {

        // Here starts the words!
        words_util in_w(vp.words);

        std::vector<word_t> out_words;
        std::array<bool, 9999> ignore_groups = { false };
        words_parser out_w(out_words);
        out_w.import_solid_groups(in_w, ignore_groups);
        out_w.import_solid_words(in_w, ignore_groups);
        out_w.import_remaining_words(in_w, ignore_groups);
        out_w.import_vocab_flags(vocab_bin);

        std::sort(out_words.begin(), out_words.end(), compare_words);

        uint8_t word_char = 'a';
        uint16_t words_offset = 0;

        int words_not_a = 0; // (NOT NEEDED)

        word_offsets[0] = this->base + this->offs;

        for (const word_t& word : out_words) {
            if (word_char != word.token[0]) {
                word_char = word.token[0];
                write_u8(0); // terminate
                words_not_a--; // subtract doubly counted (NOT NEEDED)
                word_offsets[word_char - 'a'] = this->base + this->offs;
            }

            if (word_char != 'a') {
                words_not_a++; // (NOT NEEDED)
            }

            uint8_t string_size = (uint16_t)word.token.length() + 1 + 3;
            write_u8(string_size);

            // Serialize with extra flags from vocab.bin
            uint16_t word_group = word.group;

            word_group |= word.flags;

            write_u16(word_group);
            write_sz(word.token);

            max_groups = std::max(max_groups, word.group);
        }

        write_u8(0); // terminate

        max_groups++;

        // NOTE; original gbagi accidentally reserved an extra byte pr word not starting with a, 
        // replicated for binary comparison purposes; all code commented "(NOT NEEDED)" is related
        // and can be removed
        for (int i = 0; i < words_not_a; i++) {
            write_u8(0); // (NOT NEEDED)
        }
    }

    void write_logic_words(const agi_parser& vp, int max_groups, std::array<uint32_t, 256>& logic_word_offsets) {
        // Array of (max_groups >> 2) bytes pr logic, 2 bits per group = 4 groups per byte.
        // Encoded as two bits pr group; 0x1 = "main word", 0x2 = "more word"
        std::vector<uint8_t> logic_words0;

        for (int lognum = 0; lognum < 256; lognum++) {
            const logic_t& state = vp.logics[lognum];

            if (state.groups.empty() && state.more_groups.empty()) {
                // if no words in room/logic, set offset = 0 and skip
                continue;
            }

            logic_word_offsets[lognum] = this->base + this->offs;

            std::vector<uint8_t> logic_words((max_groups >> 2) + 1);

            for (const uint16_t& g : state.groups) {
                if (g == 0 || g == 9999 || g >= max_groups) {
                    continue;
                }

                uint16_t group_bitofs = g >> 2;
                logic_words[group_bitofs] |= 0x1 << ((g & 3) << 1); // 0x1 = referenced in main logic and should always appear
            }

            for (const uint16_t& g : state.more_groups) {
                if (g == 0 || g == 9999 || g >= max_groups) {
                    continue;
                }

                uint16_t group_bitofs = g >> 2;
                logic_words[group_bitofs] |= 0x2 << ((g & 3) << 1); // 0x2 = referenced indirectly and should appear under "More"
            }

            // Amend words from logic 0 as "more words" in all rooms. Logic 0 only has "main words", so the shift-left turns them into "more" words.
            if (lognum == 0) {
                logic_words0 = logic_words;
            } else {
                if (!logic_words0.empty()) {
                    for (size_t j = 0; j < logic_words.size(); j++) {
                        logic_words[j] |= logic_words0[j] << 1;
                    }
                }
            }

            write_buf(logic_words);
        }
    }

    void output_game(const agi_parser& vp, const words_util& vocab_bin, std::array<uint32_t, 14>& ptrs) {

        ptrs[1] = this->base + this->offs;

        std::array<std::array<uint32_t, 256>, 4> dir_offsets = { 0 };

        for (int volnum = 0; volnum < 16; volnum++) {
            write_voldata(vp.dirs, volnum, offs, dir_offsets);
        }
        
        ALIGN(fout);

        ptrs[2 + 0] = this->base + this->offs;
        write_u32_array(dir_offsets[0]);

        ptrs[2 + 1] = this->base + this->offs;
        write_u32_array(dir_offsets[1]);

        ptrs[2 + 2] = this->base + this->offs;
        write_u32_array(dir_offsets[2]);

        ptrs[2 + 3] = this->base + this->offs;
        write_u32_array(dir_offsets[3]);

        ALIGN(fout);

        // Write inventory item names
        ptrs[6] = this->base + this->offs;

        std::array<uint32_t, 256> object_offsets = { 0 };
        std::array<uint8_t, 256> object_rooms = { 0 };
        write_inventory_items(vp, object_offsets, object_rooms);

        ALIGN(fout);

        // Write inventory item name pointers
        ptrs[7] = this->base + this->offs;
        write_u32_array(object_offsets);

        // Write inventory item room numbers
        ptrs[8] = this->base + this->offs;
        write_u8_array(object_rooms);

        // Write tokens (GameEnts->wordData)
        ptrs[9] = this->base + this->offs;

        std::array<uint32_t, 26> word_offsets = { 0 };
        int max_groups = 0;

        write_word_data(vp, vocab_bin, word_offsets, max_groups);

        ALIGN(fout);

        // Write starting token offsets for all 26 letters (GameEnts->words)
        ptrs[10] = this->base + this->offs;
        write_u32_array(word_offsets);

        ALIGN(fout);

        // Write word group flags pr logic (GameEnts->wordFlags)
        ptrs[11] = this->base + this->offs;

        std::array<uint32_t, 256> logic_word_offsets = { 0 };
        write_logic_words(vp, max_groups, logic_word_offsets);
        ALIGN(fout);

        // Write pointers to each logic's word array (GameEnts->logWords)
        ptrs[12] = this->base + this->offs;
        write_u32_array(logic_word_offsets);

        ptrs[13] = max_groups;
    }

    void output_games(const std::vector<agi_parser>& games, const words_util& vocab_bin) {
        // Reset this->offs to be relative to the game data from where pointer offsets are computed
        this->offs = 0;

#define IDSIZE	24
        char agiid[] = "GBAGI 1.0 '''BRIPRO'''\0";

        write_sz(agiid); // 23 bytes
        write_u8(0); // +1 = 24 bytes header

        write_u8((uint8_t)games.size());
        for (int i = IDSIZE + 1; i < 0x20; i++)
            write_u8(0);

        long giPos = ftell(fout);

        for (size_t i = games.size() * 80; i > 0; i--)
            write_u8(0);

        for (size_t i = 0; i < games.size(); i++) {
            std::array<uint32_t, 14> ptrs = { 0 };
            this->output_game(games[i], vocab_bin, ptrs);
            this->write_game_header(giPos, ptrs, games[i].title);
            giPos += 80;
        }
    }

    void write_game_header(size_t giPos, const std::array<uint32_t, 14>& ptrs, const std::string& title) {
        long fstart = ftell(fout);
        long pstart = this->offs;

        fseek(fout, giPos, SEEK_SET);
        write_u8('A');
        write_u8('G');
        write_u8('i');
        write_u8(0); // version major
        write_u16(0);// version minor
        write_sz(title);
        write_buf(std::vector<uint8_t>(26 - (title.length() + 1)));

        // NOTE; not saving first (+1), not saving last(-8), effectively writing ptrs 1-12
        fwrite(&ptrs[1], sizeof(ptrs) - 8, 1, fout);

        fseek(fout, fstart, SEEK_SET);
        this->offs = pstart;
    }
};
