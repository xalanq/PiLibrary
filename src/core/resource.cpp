// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <cstdio>
#include <sys/stat.h>

#include <core/resource.h>

namespace R {
    size_t fileSize(const xstring &path) {
        struct stat statbuff;
        if (stat(path.c_str(), &statbuff) >= 0)
            return statbuff.st_size;
        return 0;
    }

    bool add(const xstring &path, const char *data, const size_t &size) {
        FILE *fio = fopen(path.c_str(), "wb");
        if (fio == NULL)
            return false;
        if (fwrite(data, size, 1, fio) != 1)
            return false;
        fclose(fio);
        return true;
    }

    char* get(const xstring &path, size_t &size) {
        size = 0;
        FILE *fio = fopen(path.c_str(), "rb");
        if (fio == NULL)
            return nullptr;
        size = fileSize(path);
        char *buf = new char[size];
        if (fread(buf, size, 1, fio) != 1) {
            delete buf;
            buf = nullptr;
        }
        fclose(fio);
        return buf;
    }
}
