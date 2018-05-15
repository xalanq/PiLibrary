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

Book::xint Book::getAmount() const {
    return amount;
}

void Book::setAmount(const xint &value) {
    amount = value;
}

Book::xstring Book::getPosition() const {
    return position;
}

void Book::setPosition(const xstring &value) {
    position = value;
}

Book::xint Book::getStarCount() const {
    return starCount;
}

void Book::setStarCount(const xint &value) {
    starCount = value;
}