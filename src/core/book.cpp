// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/book.h>

void Book::addResource(const xstring &name, const xstring &path) {
    resource[name] = path;
}

void Book::removeResource(const xstring &name) {
    auto it = resource.find(name);
    if (it != resource.end()) {
        R::remove(it->second);
    }
}
