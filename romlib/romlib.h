#pragma once

#include <cstdint>
#include <string>
#include <vector>

#define SINGLE_DIR	1
#define SINGLE_DIR_DIRS	16 // whether single dir filename is <ID>+"dir" (f.ex "kqdir") or "dirs"

#define PACKED_DIRS	2
#define ENCRYPT_OBJ	4
#define AMIGA		8

typedef struct {
    uint32_t flags;
    uint16_t major, minor;
} GVER;

struct dir_ent_t {
    uint8_t vol = 0;
    uint8_t vi = 0; // vol index read from vol ent header? whats difference from "vol"
    std::vector<uint8_t> encbuffer; // direct pointer to vol, compressed or not
    std::vector<uint8_t> decbuffer; // same as encbuffer if uncompressed, otherwise the uncompressed data
};

struct dir_info_t {
    std::vector<dir_ent_t> ents;
};

struct word_t {
    std::string token; // from words.tok
    int group = 0; // from words.tok
    int flags = 0; // from vocab.bin
};

struct inventory_object_t {
    std::string name;
    uint8_t room;
};

#define ALIGN(f)\
while((offs&3)!=0) {\
     	fputc(0,f);\
        offs++; \
    }

bool read_binary(const std::string& filename, std::vector<uint8_t>& data);
std::string i2s(int i);
void DecryptBlock(std::vector<uint8_t>& data, size_t offset, size_t len);
bool compare_words(word_t& i, word_t& j);
bool exists(const std::string& filename);
void get_files(const std::string& path, std::vector<std::string>& files);
bool ends_with(const std::string& stack, const std::string& needle);

// romlib uses utf8 strings internally and provides these macros to convert between windows TCHAR apis
#if defined(_UNICODE)
std::string T2U(const std::wstring& str);
std::wstring U2T(const std::string& str);
#else
std::string T2U(const std::string& str);
std::string U2T(const std::string& str);
#endif