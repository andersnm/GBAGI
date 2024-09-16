#pragma once

#include <array>
#include "romlib.h"
#include "vector_reader.h"
#include "words_util.h"

struct words_parser {
    std::vector<word_t>& words;

    words_parser(std::vector<word_t>& words) : words(words) {
    }

    // Load words.tok from an AGI game
    bool load_words(const std::string& filename) {
        std::vector<uint8_t> data;
        if (!read_binary(filename, data)) {
            return false;
        }

        vector_reader header_r(data);

        for (int i = 0; i < 26; i++) {
            uint16_t words_offset = header_r.read_uint16_be();
            if (words_offset == 0)
                continue;

            vector_reader word_r(data, words_offset);
            std::array<char, 128> wordbuf = { 0 };
            do {
                uint8_t wordpos = word_r.read_uint8();

                for (;;) {
                    uint8_t msg = word_r.read_uint8();

                    wordbuf[wordpos] = (msg & 0x7F) ^ 0x7F;
                    wordpos++;

                    if ((msg & 0x80) != 0) {
                        break;
                    }
                }

                wordbuf[wordpos++] = 0;

                uint16_t g = word_r.read_uint16_be();
                if (g != 1 && g != 9999) {
                    word_t wt;
                    wt.token = &wordbuf[0];
                    wt.group = g;
                    words.push_back(wt);
                }

            } while (word_r.peek_uint8());
        }

        return true;
    }

    // Load GBAGI's vocab.bin containing common words, using group field as a flag for noun/verb
    bool load_vocab(const std::string& filename) {
        std::vector<uint8_t> data;
        if (!read_binary(filename, data)) {
            return false;
        }

        vector_reader r(data);

        while (r.peek_uint8()) {
            int len = r.read_uint8(); // Not used anymore
            int group = r.read_uint16();

            word_t w;
            w.group = group;
            w.token = r.read_sz();
            words.push_back(w);
        }

        return true;
    }

    void add_word(int group, const std::string& word) {
        word_t w;
        w.token = word;
        w.group = group;
        this->words.push_back(w);
    }

    void set_flag(int group, int flags) {
        for (word_t& word : this->words) {
            if (word.group == group) {
                word.flags |= flags;
            }
        }
    }

    //void ClearGroup(int group) {
    //    for (word_t& w : this->words) {
    //        if (w.group == group) {
    //            w.group = 0;
    //        }
    //    }
    //}

    void import_solid_groups(const words_util& in_words, std::array<bool, 9999>& ignore_groups)
    {
#define SG_TOTAL 8
#define SG_WORDMAX 20

        static const char* solidGroups[SG_TOTAL][SG_WORDMAX] = {
            {"enter","exit", ""},
            {"taste", "eat", "drink", ""},
            {"hug", "kiss", ""},
            {"cabinet","closet","cupboard","wardrobe", ""},
            {"bureau","chest","dresser", ""},
            {"purchase", "pay", ""},
            {"car", "taxi", ""},
            {""}
        };

        int set = 0;

        while (solidGroups[set][0][0]) {
            for (int idx = 0; ; idx++) {
                const char* s = solidGroups[set][idx];
                if (!s[0]) break;

                int group = in_words.find_word_group(s);
                if (group != 0) {
                    this->add_word(group, s);
                    ignore_groups[group] = true;
                }
            }
            set++;
        }
    }

    void import_solid_words(const words_util& in_words, std::array<bool, 9999>& ignore_groups)
    {
        int idx = 0, group;
        const char* s;

        static const char* solidWords[] = {
            "look","marry", "take", "button", "phone", "hallway", "drinks", "hole", "cigarette",
            "yes", "no", "wall", "area", "wallet", "ground", "stairs", "window", "open", "move", "climb", "talk",
            "garbage", "ledge", "under", "over", "drawer", "carpet", "out", "key", "give",
            "lie down", "touch", "empty", "find", "behind", "door", "clothes", "lock", "break",
            "listen", "table", "shelf", "book", "thank you", "box", "jump", "man", "woman",
            "start", "ignite", "fire", "meter", "lips", "basket", "wine", "food", "steal", "knife",
            "cut", "sink", "sit down", "hit", "knock on", "cover", "bottle", "television", "put on",
            "ashtray", "bill", "pass", "ladder", "capsule", "negotiate", "flower", "elevator",
            "paper", "ken sent me", "feel", "bartender", "sleep", "pills", "water", "rope",
            "ring", "blow up", "release", "booze", "remote control", "whiskey", "bathroom",  "mat",
            "notes", "wash", "candy", "doll", "magazine", "old", "building", "get up", "change",
            "channel", "jukebox", "wipe", "attatch", "clothes line", "decrease", "increase",
            "spray", "bar", "church", "lint", "blackjack", "slots" , "dog", "pole", "rough",
            "plants", "boo", "music", "junk food", "gamble", "lever", "split", "rules",
            "swim suit", "moose", "undress", "dress", "artwork", "fan", "yourself", "fire hydrant",
            "enjoy", "disc jockey", "roof", "weapon", "onto", "person", "watch", "waiter", "spit",
            "shout", "dive", "under water", "bucket", "leg", "bubbles", "credit card", "pause",
            "suit", "cooler", "information", "cheer", "stop", "inventory", "crack", "crap", "leak",
            "screw", "fart", "boobs", "ass", "dong", "up yours", "business card", "work",
            "computer console", "transporter", "screen", "fuel pump", "close", "catch", "rock",
            "push", "position", "forest", "help", "swim", "climb", "ground", "plants", "smell", "stand up",
            "knife", "torch", "soup", "catch", "opening", "bag", "get in", "tree", "key", "light",
            "knight", "pump", "eat", "silence", "name", "bird", "feed", "diamond", "throw", "drop",
            "cast", "fence", "stove", "fly", "move", "woman", "remove", "river", "guitar", "initialize",
            "hat", "chair", "keyhole", "duck", "suite", "body", ""
        };

        for (idx = 0; ; idx++) {
            s = solidWords[idx];
            if (!s[0]) {
                break;
            }

            group = in_words.find_word_group(s);
            if (group != 0) {
                if (!ignore_groups[group]) {
                    this->add_word(group, s);
                    ignore_groups[group] = true;
                }
            }
        }
    }

    void import_remaining_words(const words_util& in_words, std::array<bool, 9999>& ignore_groups)
    {
        for (int i = 1; i < 9999; i++) {

            if (ignore_groups[i]) {
                continue;
            }

            std::vector<std::string> words_in_group;
            in_words.find_words_in_group(i, words_in_group);

            if (words_in_group.size() > 0) {
                this->add_word(i, words_in_group[0]);
            }

            if (words_in_group.size() > 1) {
                this->add_word(i, words_in_group[1]);
            }
        }
    }

    void import_vocab_flags(const words_util& vocab_bin) {
        for (const word_t& word : this->words) {

            int q = vocab_bin.find_word_group_x(word.token.c_str());
            if ((q != -1) && (q & 0x80)) {
                // all words in this group gets the 0x8000 flag
                set_flag(word.group, 0x8000);
            }
        }
    }

};