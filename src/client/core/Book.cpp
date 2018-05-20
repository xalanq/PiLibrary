// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/core/Book.h>
#include <core/resource.h>

Book::xint Book::getAmount() const {
    return amount;
}

Book& Book::setAmount(const xint &value) {
    amount = value;
    return *this;
}

Book::xstring Book::getPosition() const {
    return position;
}

Book& Book::setPosition(const xstring &value) {
    position = value;
    return *this;
}

Book& Book::addResource(const xstring &name, const xstring &path) {
    resource[name] = path;
    return *this;
}

Book& Book::removeResource(const xstring &name) {
    auto it = resource.find(name);
    if (it != resource.end()) {
        R::remove(it->second);
    }
    return *this;
}
    
Book& Book::setFromPtree(const ptree &pt) {
    BookBrief::setFromPtree(pt);
    setAmount(pt.get<xint>("amount", 0));
    setPosition(pt.get<xstring>("position", ""));
    setStarCount(pt.get<xint>("starCount", 0));
    return *this;
}

Book Book::fromPtree(const ptree &pt) {
    Book book;
    book.setFromPtree(pt);
    return std::move(book);
}

const Book& Book::unknown() {
    static Book book;
    static bool init {false};
    if (!init) {
        book.setTitle("Unknown");
        init = true;
    }
    return book;
}
