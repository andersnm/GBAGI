#pragma once
#include <array>
#include "romlib.h"
#include "vector_reader.h"
#include "decompress.h"

// Games that use version 3 of the AGI interpreter will have a single file called *DIR where the star is the initials of the game (eg.BC, GR, MH2, MH1, KQ4).
// This single file is basically the four version 2 files joined together except that it has an 8 byte header giving the position of each directory within the single file.
// Additional note; Amiga games observed to just have "dirs" file

struct dir_parser {
    std::string path;
    std::string prefix;

    dir_parser(const std::string& path, const std::string& prefix) {
        this->path = path;
        this->prefix = prefix;
    }

    void load_vols(std::array<std::vector<uint8_t>, 16>& vols) {
        for (int volnum = 0; volnum < 16; volnum++) {
            read_binary(this->path + prefix + "vol." + i2s(volnum), vols[volnum]);
        }
    }

    bool load_dir(std::array<dir_info_t, 4>& dirs, uint32_t flags) {
        if ((flags & SINGLE_DIR) != 0) {
            return this->load_single_dir(dirs, flags);
        } else {
            return this->load_file_dirs(dirs, flags);
        }
    }

    bool load_single_dir(std::array<dir_info_t, 4>& dirs, uint32_t flags) {
        std::array<std::vector<uint8_t>, 16> vols;
        load_vols(vols);

        std::vector<uint8_t> dirbuffer;
        std::string dirfile;
        if ((flags & SINGLE_DIR_DIRS)) {
            dirfile = "dirs";
        } else {
            dirfile = prefix + "dir"; // prefix is game-id, not like "logdir"
        }

        if (!read_binary(this->path + dirfile, dirbuffer)) {
            return false;
        }

        if (dirbuffer.size() < 8) {
            return false;
        }

        vector_reader dir_r(dirbuffer);

        uint16_t offs0 = dir_r.read_uint16();
        uint16_t offs1 = dir_r.read_uint16();
        uint16_t offs2 = dir_r.read_uint16();
        uint16_t offs3 = dir_r.read_uint16();

        parse_dir(0, &dirbuffer[offs0], offs1 - offs0, vols, dirs[0], flags);
        parse_dir(1, &dirbuffer[offs1], offs2 - offs1, vols, dirs[1], flags);
        parse_dir(2, &dirbuffer[offs2], offs3 - offs2, vols, dirs[2], flags);
        parse_dir(3, &dirbuffer[offs3], dirbuffer.size() - offs3, vols, dirs[3], flags);

        return true;
    }

    bool load_file_dirs(std::array<dir_info_t, 4>& dirs, uint32_t flags) {
        static std::string dirNames[] = { "logdir", "picdir", "viewdir", "snddir" };

        std::array<std::vector<uint8_t>, 16> vols;
        load_vols(vols);

        for (int dirnum = 0; dirnum < 4; dirnum++) {
            std::string filename = this->path + dirNames[dirnum];
            std::vector<uint8_t> dirbuffer;
            if (!read_binary(filename, dirbuffer)) {
                return false;
            }

            if (!parse_dir(dirnum, &dirbuffer[0], dirbuffer.size(), vols, dirs[dirnum], flags)) {
                return false;
            }
        }

        return true;
    }

    bool parse_dir(int dirnum, uint8_t* dirbuffer, int dirsize, const std::array<std::vector<uint8_t>, 16>& vols, dir_info_t& dir, uint32_t flags) {
        // dirs on mac are padded with zeros at the end, object file too
        // remove padded zeros at end, first do a mod-3-trim, assuming zeros
        dirsize -= dirsize % 3;

        while (dirsize >= 3 && !dirbuffer[dirsize - 3] && !dirbuffer[dirsize - 2] && !dirbuffer[dirsize - 1]) {
            dirsize -= 3;
        }

        int totalEnts = dirsize / 3;
        if (totalEnts > 256) totalEnts = 0;
        bool packed_dirs = (flags & PACKED_DIRS) != 0;

        for (int i = 0; i < totalEnts; i++) {
            // 24bit pr entry, 20 bit offset, 4bit vol
            uint8_t b0 = dirbuffer[0];
            uint8_t b1 = dirbuffer[1];
            uint8_t b2 = dirbuffer[2];
            dirbuffer += 3;

            uint32_t value = (b0 << 16) | (b1 << 8) | b2;
            dir_ent_t ent;
            if (value != 0xFFFFFF) {
                int offset = value & 0x0FFFFF;;
                ent.vol = (value >> 20) & 0xF;

                parse_ent_from_vol(ent, offset, vols, packed_dirs);
                unpack_ent(dirnum, ent);
            } else {
                ent.vol = -1;
            }

            dir.ents.push_back(ent);
        }

        return true;
    }

    bool parse_ent_from_vol(dir_ent_t& ent, size_t offset, const std::array<std::vector<uint8_t>, 16>& vols, bool packed_dirs) {
        if (ent.vol >= vols.size()) {
            return false;
        }

        const std::vector<uint8_t>& volbuffer = vols[ent.vol];
        if (volbuffer.empty()) {
            return false;
        }

        if (offset >= volbuffer.size()) {
            return false;
        }

        vector_reader vol_r(volbuffer, offset);

        uint16_t header = vol_r.read_uint16();
        if (header != 0x3412) {
            // kq3_amiga and mh1_dos got some of these
            return false;
        }

        ent.vi = vol_r.read_uint8();
        uint16_t declen = vol_r.read_uint16();

        uint16_t enclen;
        if (packed_dirs) {
            enclen = vol_r.read_uint16();
        } else {
            enclen = declen;
        }

        ent.decbuffer = std::vector<uint8_t>(declen);
        ent.encbuffer = std::vector<uint8_t>(volbuffer.begin() + vol_r.offset(), volbuffer.begin() + vol_r.offset() + enclen);
        return true;
    }

    bool unpack_ent(int dirnum, dir_ent_t& ent) {

        if (ent.decbuffer.size() == 0) {
            return true;
        }

        if (ent.encbuffer.size() == ent.decbuffer.size()) { // not compressed
            if (dirnum == 0) { // logic
                //The layout of a logic buffer goes like:
                //    uint16_t bytecode_size;
                //    uint8_t  bytecodes[bytecode_size];
                //    uint8_t  strings_count;
                //    uint16_t strings_size;
                //    uint16_t strings_flags[strings_count];
                //    uint8_t strings_data[strings_size - sizeof(strings_flags) - sizeof(strings_size)];
                // Only the strings_data is encrypted

                vector_reader log_r(ent.encbuffer);

                uint16_t bytecode_size = log_r.read_uint16();
                if (bytecode_size + 2U + 1U > ent.encbuffer.size()) {
                    return false;
                }

                log_r.skip(bytecode_size);

                uint8_t strings_count = log_r.read_uint8();
                if (strings_count) {
                    uint16_t strings_size = log_r.read_uint16();
                    log_r.skip(strings_count * 2);
                    size_t decrypt_length = strings_size - (strings_count * 2) - 2;
                    DecryptBlock(ent.encbuffer, log_r.offset(), decrypt_length);
                }
            }

            ent.decbuffer = ent.encbuffer;
        } else if (ent.vi & 0x80) { // compressed picture file
            PIC_expand(&ent.encbuffer[0], &ent.decbuffer[0], ent.decbuffer.size());
        } else { // compressed LZW
            LZW_expand(&ent.encbuffer[0], &ent.decbuffer[0], ent.decbuffer.size());
        }

        return true;
    }
};
