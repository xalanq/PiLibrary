// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <cstdio>
#include <sys/stat.h>

#include <boost/filesystem.hpp>

#include <core/resource.h>

Resource::Resource(char *data, const size_t &size) :
    data(data), 
    size(size) { }

Resource::Resource(const Resource &r) :
    data(r.data),
    size(r.size) { }

Resource::Resource(Resource &&r) :
    data(std::move(r.data)),
    size(std::move(r.size)) {
    r.data = nullptr;
    r.size = 0;
}

Resource& Resource::operator = (const Resource &r) {
    data = r.data;
    size = r.size;
    return *this;
}

Resource& Resource::operator = (Resource &&r) {
    data = std::move(r.data);
    size = std::move(r.size);
    r.data = nullptr;
    r.size = 0;
    return *this;
}

char* Resource::getData() {
    return data;
}

const char* Resource::getData() const {
    return data;
}

Resource& Resource::setData(char *data) {
    this->data = data;
    return *this;
}

size_t Resource::getSize() const {
    return size;
}

Resource& Resource::setSize(const size_t &size) {
    this->size = size;
    return *this;
}

void Resource::clean() {
    if (data) {
        size = 0;
        delete[] data;
    }
}

size_t Resource::fileSize(const xstring &path) {
    struct stat statbuff;
    if (stat(path.c_str(), &statbuff) >= 0)
        return statbuff.st_size;
    return 0;
}

bool Resource::add(const xstring &path, const Resource &file) {
    boost::filesystem::path p(path);
    boost::filesystem::create_directories(p.parent_path());
    FILE *fio = fopen(path.c_str(), "wb");
    if (fio == NULL)
        return false;
    if (fwrite(file.getData(), file.getSize(), 1, fio) != 1)
        return false;
    fclose(fio);
    return true;
}

Resource Resource::get(const xstring &path) {
    FILE *fio = fopen(path.c_str(), "rb");
    if (fio == NULL)
        return nullptr;
    Resource file;
    auto size = fileSize(path);
    file.setSize(size)
        .setData(new char[size]);
    if (fread(file.getData(), size, 1, fio) != 1)
        file.clean();
    fclose(fio);
    return file;
}

Resource Resource::copy(const Resource &file) {
    char *s = new char[file.size];
    memcpy(s, file.data, file.size);
    return Resource(s, file.size);
}
