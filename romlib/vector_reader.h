#pragma once

#include <vector>
#include <cstdint>
#include <string>

struct vector_reader {
    const std::vector<uint8_t>& bytes;
    std::vector<uint8_t>::const_iterator it;

    vector_reader(const std::vector<uint8_t>& bytes, size_t offset = 0) : bytes(bytes) {
        reset(offset);
    }

    void reset(size_t offset = 0) {
        it = this->bytes.begin() + offset;
    }

    size_t offset() {
        return std::distance(bytes.begin(), it);
    }

    bool eof() {
        return it >= this->bytes.end();
    }

    void skip(size_t len) {
        it += len;
    }

    uint8_t peek_uint8() {
        return *it;
    }

    uint8_t read_uint8() {
        uint8_t b = *it;
        it++;
        return b;
    }

    uint16_t read_uint16() {
        uint8_t b0 = read_uint8();
        uint8_t b1 = read_uint8();
        return (b0)+(b1 << 8);
    }

    uint16_t read_uint16_be() {
        uint8_t b0 = read_uint8();
        uint8_t b1 = read_uint8();
        return (b1)+(b0 << 8);
    }

    std::string read_sz() {
        std::string str;
        for (;;) {
            uint8_t b = read_uint8();
            if (b == 0) {
                break;
            }

            str.push_back(b);
        }

        return str;
    }
};