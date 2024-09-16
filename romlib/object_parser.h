#pragma once

#include "romlib.h"
#include "vector_reader.h"

struct object_parser {
    std::vector<inventory_object_t>& objects;

    object_parser(std::vector<inventory_object_t>& objects) : objects(objects) {
    }

    bool load_object(const std::string& filename, uint8_t version_flags)
    {
        // NOTE; objects file is padded with zeros on mac

        std::vector<uint8_t> data;
        if (!read_binary(filename, data)) {
            return false;
        }

        vector_reader header_r(data);

        uint16_t q = header_r.read_uint16();
        if (q > data.size() || q < 3)// || (gi->version->ver.flags & ENCRYPT_OBJ))
            DecryptBlock(data, 0, data.size());

        header_r.reset();

        uint16_t header_size = header_r.read_uint16();
        uint8_t max_objects = header_r.read_uint8();
        if ((version_flags & AMIGA) != 0) {
            header_r.read_uint8();
        }

        // The offsets are relative to the first object entry
        size_t names_start_offset = header_r.offset();
        uint16_t header_end_offset = header_r.offset() + header_size;

        while (header_r.offset() < header_end_offset) {
            uint16_t name_offset = header_r.read_uint16();
            uint8_t room = header_r.read_uint8();

            if ((version_flags & AMIGA) != 0) {
                header_r.read_uint8(); 
            }

            size_t string_offset = names_start_offset + name_offset;
            if (string_offset  > data.size()) {
                return false;
            }

            inventory_object_t inv;
            inv.name = (char*)&data[string_offset];
            inv.room = room;
            objects.push_back(inv);
        }

        return true;
    }
};