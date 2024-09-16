#pragma once
#include <cstdint>
#include "vector_reader.h"
#include "commands.h"

/* 
logic_parser works like the original makerom.cpp, but implements only the
reading/parsing/evaluation of AGI room scripts (logics), and leaves the GBAGI
binary ROM output step for later.

Purpose is to collect all words used directly or indirectly in each room,
specifically to collect all arguments passed to the "said()" AGI function and
classify the words as either "main word" or "more word".

- "main words" always appear in the input boxes in GBAGI.
- "more words" appears when pushing the "More" button in GBAGI.

A logic_parser instance evalutes a single room script. Manages
additional logic_parser instances internally to evaluate "call()"
instructions. Nested instances return output in the root instance's
"more_groups" array.

The parser handles the "call(var)" instruction, which evaluates the logic
number stored in a variable. Because variables are variable, all
assignments are tracked and the "call(var)" instruction is evaluated once
for each unique non-zero assignments to that variable.
*/

struct logic_t {
    std::vector<uint8_t> code;
    std::vector<uint16_t> groups;
    std::vector<uint16_t> more_groups;
};

struct logic_state_t {
    uint8_t vVals[256][8] = { 0 }; // 256 variables, tracks up to 8 different non-zero assignments
};

struct logic_parser {

    logic_parser* parent;
    std::array<logic_t, 256>& logics;
    std::array<bool, 256> visited = { false };
    logic_t& logic;
    uint8_t lognum;

    logic_parser(logic_parser* parent, std::array<logic_t, 256>& logics, uint8_t lognum)
        : parent(parent)
        , logics(logics)
        , lognum(lognum)
        , logic(logics[lognum])
    {
    }

    bool parse_logic(logic_state_t& state) {
        if (this->get_root()->visited[lognum]) {
            return true;
        }

        this->get_root()->visited[lognum] = true;

        if ((!this->logic.groups.empty() || !this->logic.more_groups.empty()) && this->parent == nullptr) {
            return true;
        }

        vector_reader logic_r(this->logic.code);
        return this->parse_scope(logic_r, state);
    }

private:
    logic_parser* get_root() {
        logic_parser* root = this;
        while (root->parent) {
            root = root->parent;
        }

        return root;
    }

    void add_group(uint16_t group) {
        if (&this->get_root()->logic == &this->logics[0] || this->parent == nullptr ) {
            // All words in logic 0 and directly referenced in top-level logic are "main words"
            this->logic.groups.push_back(group);
        } else {
            // Else was referenced in a subroutine and is a "more word"
            this->logic.more_groups.push_back(group);
        }

        // Add group to all logics in call stack
        if (this->parent) {
            this->parent->add_group(group);
        }
    }

    bool parse_said(vector_reader& logic_r) {
        int saidCnt = logic_r.read_uint8();

        while (saidCnt) {
            if (logic_r.eof()) {
                return false;
            }

            uint16_t group = logic_r.read_uint16();
            this->add_group(group);

            saidCnt--;
        }

        return true;
    }

    bool parse_if(vector_reader& logic_r, logic_state_t& state) {
        int op;

        // This parses the "if" expression, ends with 0xff
        // Test expressions can contain bytecodes referencing special "test functions",
        // or special codes 0xfe = else, 0xfd = not, 0xfc = or. the "and" operator is implicit.

        for (;;) {
            if (logic_r.eof()) {
                return false;
            }

            op = logic_r.read_uint8(); // code[code_offset];
            if (op == 0xFF) break;

            if (op >= 0xFC && op <= 0xFE) {
                // skip single byte op for else|goto / not / or
            }
            else if (op == 0xE) {
                // op 0xE = said(...) - has variable number of arguments of uint16's specifying the token group from words.tok
                if (!this->parse_said(logic_r)) {
                    return false;
                }
            }
            else {
                // printf("Iffing %s\n", testCommands[op].name);
                if (op >= MAX_TESTCMD) {
                    return false;
                }
                logic_r.skip(testCommands[op].nParams);
            }
        }

        return parse_scope(logic_r, state);
    }

    bool parse_assign(vector_reader& logic_r, uint32_t end_offset, logic_state_t& state) const {
        // Keep track of every different assignment to a variable, and when the variable is called, all different assignments are evaluated, up to 8
        uint8_t varnum = logic_r.read_uint8();
        uint8_t value = logic_r.read_uint8();
        for (int i = 0; i < 8; i++) {
            if (state.vVals[varnum][i] == value)
                break;
            if (!state.vVals[varnum][i]) {
                state.vVals[varnum][i] = value;
                break;
            }
        }

        return true;
    }

    bool parse_call(vector_reader& logic_r, logic_state_t& state) {
        uint8_t lognum = logic_r.read_uint8();

        logic_parser lp(this, this->logics, lognum);
        lp.parse_logic(state);
        return true;
    }

    bool parse_callv(vector_reader& logic_r, logic_state_t& state) {
        uint8_t varnum = logic_r.read_uint8();
        for (int i = 0; i < 8; i++) {
            if (!state.vVals[varnum][i]) break;

            int lognum = state.vVals[varnum][i];
            logic_parser lp(this, this->logics, lognum);
            lp.parse_logic(state);
        }

        return true;
    }

    bool parse_scope(vector_reader& logic_r, logic_state_t& state) {
        if (logic_r.eof()) {
            return true;
        }

        uint16_t scope_size = logic_r.read_uint16();
        uint32_t end_offset = logic_r.offset() + scope_size;

        while (logic_r.offset() < end_offset) {
            uint8_t op = logic_r.read_uint8();
            switch (op) {
            case 3: // assign
                this->parse_assign(logic_r, end_offset, state);
                break;
            case 22: // call(const)
                this->parse_call(logic_r, state);
                break;
            case 23: // call(var)
                this->parse_callv(logic_r, state);
                break;
            case 0xFF:
                this->parse_if(logic_r, state);
                break;
            default:
                logic_r.skip(agiCommands[op].nParams);
                break;
            }
        }

        //printf("group in log %i(", num);
        //for (auto g : groups) printf("%i ", g);
        //printf(")\n");

        if (logic_r.offset() > end_offset) {
            // Recover cleanly from SQ 1 Amiga logic 99 missing argument in quit(), the last statement in an if-block
            logic_r.reset(end_offset);
        }

        return true;
    }
};
