// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/book.h>

void Book::addResource(const string &name, const string &path) {
    resource[name] = path;
}

void Book::removeResource(const string &name) {
    auto it = resource.find(name);
    if (it != resource.end()) {
        R::remove(it->second);
    }
}
