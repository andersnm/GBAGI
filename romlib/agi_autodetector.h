#pragma once

struct agi_autodetector {

    bool autodetect_dir_flags(const std::string& path, uint32_t& flags, std::string& prefix) {

        // Is there a file "dirs"? -> single file dirs
        // No -> Is there a file "logdir" -> multiple dir files
        // No -> Is there a file that ends with "*dir", and also "*vol.0" -> single file dirs with prefix
        // No -> Not an AGI game

        if (exists(path + "dirs") || exists(path + "DIRS")) {
            flags |= SINGLE_DIR | SINGLE_DIR_DIRS;
            return true;
        }
        else if (exists(path + "logdir") || exists(path + "LOGDIR")) {
            // dirs are contained in multiple files
            return true;
        }
        else {
            std::vector<std::string> files;
            get_files(path, files);

            for (const auto& file : files) {
                if (ends_with(file, "dir") || ends_with(file, "DIR")) {
                    // TODO; can also check we have prefix "vol" file
                    prefix = file.substr(0, file.length() - 3);
                    flags |= SINGLE_DIR;
                    return true;
                }
            }

            return false;
        }
    }

    bool autodetect_amiga(const std::string& path, uint32_t& flags) {
        // Try read the "objects" file, it has 4 bytes per record on amiga, 3 bytes elsewhere
        std::vector<inventory_object_t> items;
        object_parser op(items);
        if (op.load_object(path + "object", 0)) {
            // Loads without the AMIGA flag
            return true;
        }
        else if (op.load_object(path + "object", AMIGA)) {
            flags |= AMIGA;
            return true;
        }
        else {
            // cant load either type of object file, not an agi game
            return false;
        }
    }

    bool autodetect_packed_dir(const std::string& path, const std::string& prefix, uint32_t& flags) {
        // Try unpack all dirs and vols. Assume no packing if unpacking fails.

        std::array<dir_info_t, 4> dirs;
        dir_parser dp(path, prefix);

        if (dp.load_dir(dirs, flags | PACKED_DIRS)) {
            flags |= PACKED_DIRS;
            return true;
        }

        return true;
    }

    bool autodetect_flags(const std::string& path, uint32_t& flags, std::string& prefix) {
        if (!autodetect_dir_flags(path, flags, prefix)) {
            // no known types of dir/dirs
            return false;
        }

        if (!autodetect_amiga(path, flags)) {
            // could not parse "object" file as neither dos nor amiga
            return false;
        }

        if (!autodetect_packed_dir(path, prefix, flags)) {
            // could not parse dirs with or without packed field
            return false;
        }

        return true;
    }
};
