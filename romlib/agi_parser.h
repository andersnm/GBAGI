#pragma once

#include "romlib.h"
#include "commands.h"
#include "dir_parser.h"
#include "words_parser.h"
#include "object_parser.h"
#include "logic_parser.h"
#include "agi_autodetector.h"

struct agi_parser {

    uint32_t flags = 0;
    std::string title;
    std::string path;
    std::string vID;

    std::array<dir_info_t, 4> dirs;
    std::vector<word_t> words;
    std::vector<inventory_object_t> objects;
    std::array<logic_t, 256> logics;

    bool load_game(const std::string& path, const std::string& title) {
        this->path = path;
        this->title = title;

        if (!ends_with(this->path, "/") && !ends_with(this->path, "\\")) {
            this->path += "/";
        }

        if (this->title.size() >= 26) {
            return false;
        }

        agi_autodetector aa;
        if (!aa.autodetect_flags(this->path, this->flags, this->vID)) {
            return false;
        }

        dir_parser dp(this->path, this->vID);
        dp.load_dir(this->dirs, this->flags);

        words_parser wp(this->words);
        wp.load_words(this->path + "words.tok");

        object_parser object_file(this->objects);
        object_file.load_object(this->path + "object", flags);

        for (int lognum = 0; lognum < 256; lognum++) {
            load_vol_bytes(0, lognum, logics[lognum].code);
        }

        // Original used same state for all logic evaluations
        // Might be better to have one pr logic initialized with logic 0
        logic_state_t logic_state;

        for (int lognum = 0; lognum < 256; lognum++) {
            logic_parser lp(nullptr, logics, lognum);
            lp.parse_logic(logic_state);
        }

        return true;
    }

    bool load_vol_bytes(uint8_t volnum, uint16_t entnum, std::vector<uint8_t>& bytes) const {
        const dir_info_t& dir = this->dirs[volnum]; // 0 is logic dir
        if (entnum >= dir.ents.size()) {
            return true;
        }

        const dir_ent_t& ent = dir.ents[entnum];
        if (ent.vol == -1) return true;
        if (ent.decbuffer.empty()) return true;

        bytes = ent.decbuffer;
        return true;
    }
};
