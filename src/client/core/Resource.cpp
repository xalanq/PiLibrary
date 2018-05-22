// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/core/Resource.h>

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
    if (data)
        delete[] data;
}
