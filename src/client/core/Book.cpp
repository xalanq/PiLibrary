// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/core/Book.h>

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
    
Book& Book::setFromPtree(const ptree &pt) {
    BookBrief::setFromPtree(pt);
    setAmount(pt.get<xint>("amount", 0));
    setPosition(pt.get<xstring>("position", ""));
    return *this;
}

Book Book::fromPtree(const ptree &pt) {
    Book book;
    book.setFromPtree(pt);
    return std::move(book);
}

Book& Book::updateFromPtree(const ptree &pt) {
    BookBrief::updateFromPtree(pt);
    auto amount = pt.get_optional<xint>("amount");
    auto position = pt.get_optional<xstring>("position");
    if (amount)
        setAmount(*amount);
    if (position)
        setPosition(*position);
    return *this;
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
