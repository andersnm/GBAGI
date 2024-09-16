#include <Windows.h>
#include "romlib.h"

std::wstring U2W(const std::string& str) {
    INT bufsize = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

    WCHAR wc[1024];
    if (bufsize >= sizeof(wc)) {
        return std::wstring(L"");
    }

    ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wc, sizeof(wc));
    return wc;
}

std::string W2U(const std::wstring& str) {
    INT bufsize = ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);

    CHAR utf8[1024];
    if (bufsize > sizeof(utf8)) {
        return "";
}

    ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, utf8, bufsize, NULL, NULL);
    return utf8;
}

#if defined(_UNICODE)
std::string T2U(const std::wstring& str) {
    return W2U(str);
}

std::wstring U2T(const std::string& str) {
    return U2W(str);
}
#else
std::string T2U(const std::string& str) {
    return str;
}

std::string U2T(const std::string& str) {
    return str;
}
#endif

const char szDecrypt[] = "Avis Durgan";

/*void DecryptBlock(uint8_t* start, uint8_t* end)
{
    int i = 0;
    while (start < end) {
        *start++ ^= szDecrypt[i];
        i = ((i + 1) % 11);
    }
}
*/
void DecryptBlock(std::vector<uint8_t>& data, size_t offset, size_t len)
{
    int i = 0;
    size_t end_offset = offset + len;
    while (offset < end_offset) {
        data[offset] ^= szDecrypt[i];
        offset++;
        i = ((i + 1) % 11);
    }
}

bool compare_words(word_t& i, word_t& j) {
    return (i.token < j.token);
}

std::string i2s(int i) {
    char pc[16];
    snprintf(pc, 16, "%i", i);
    return pc;
}

bool read_binary(const std::string& filename, std::vector<uint8_t>& data) {
    FILE* f;
    int l;

#if defined(_UNICODE)
    if ((f = _wfopen(U2W(filename).c_str(), L"rb")) == NULL) {
        return false;
    }
#else
    if ((f = fopen(filename.c_str(), "rb")) == NULL) {
        return false;
    }
#endif

    fseek(f, 0, SEEK_END);
    l = ftell(f);
    fseek(f, 0, SEEK_SET);

    data.resize(l);

    fread(&data.front(), data.size(), 1, f);
    fclose(f);

    return true;
}

bool exists(const std::string& filename) {
#if _UNICODE
    std::wstring wfilename = U2W(filename);
    struct _stat64i32 buffer;
    return (_wstat(wfilename.c_str(), &buffer) == 0);
#else
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
#endif
}

void get_files(const std::string& path, std::vector<std::string>& files) {
    HANDLE hFind;
    WIN32_FIND_DATA ffd;

    const std::string& find_path = path + "*";

    if ((hFind = FindFirstFile(U2T(find_path).c_str(), &ffd)) != INVALID_HANDLE_VALUE) {
        do {
            if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
                continue;
            }

            files.push_back(T2U(ffd.cFileName));
        } while (FindNextFile(hFind, &ffd));
        FindClose(hFind);
    }
}

bool ends_with(const std::string& stack, const std::string& needle) {
    if (needle.empty()) {
        return false;
    }
    return stack.find(needle, stack.size() - needle.size()) != std::string::npos;
}
