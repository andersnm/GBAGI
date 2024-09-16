#pragma once

#include "romlib.h"

struct words_util {
    const std::vector<word_t>& words;

    words_util(const std::vector<word_t>& words) : words(words) {
    }

    int find_word_group(const std::string& s) const {
        for (const auto& w : this->words) {
            if (w.token == s) {
                return w.group;
            }
        }

        return 0;
    }

    int find_word_group_x(const std::string& s) const {
        for (const auto& w : this->words) {
            // Match first part until space, "play with" in words-tok matches "play" in vocab-bin
            size_t p = 0;
            while (p < w.token.size() && p < s.size() && w.token.at(p) == s.at(p)) {
                p++;
                if (p == w.token.length() && (p == s.length() || s.at(p) == ' ')) {
                    return w.group;
                }
            }
        }

        return 0;
    }

    void find_words_in_group(int group, std::vector<std::string>& words) const {
        for (const auto& w : this->words) {
            if (w.group == group) {
                words.push_back(w.token);
            }
        }
    }

};
