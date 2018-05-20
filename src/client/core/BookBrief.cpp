// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/core/BookBrief.h>

BookBrief::xint BookBrief::getBookid() const {
    return bookid;
}

BookBrief& BookBrief::setBookid(const xint &value) {
    bookid = value;
    return *this;
}

BookBrief::xstring BookBrief::getTitle() const {
    return title;
}

BookBrief& BookBrief::setTitle(const xstring &value) {
    title = value;
    return *this;
}

BookBrief::xstring BookBrief::getAuthor() const {
    return author;
}

BookBrief& BookBrief::setAuthor(const xstring &value) {
    author = value;
    return *this;
}

BookBrief::xstring BookBrief::getISBN() const {
    return ISBN;
}

BookBrief& BookBrief::setISBN(const xstring &value) {
    ISBN = value;
    return *this;
}

BookBrief::xstring BookBrief::getPublisher() const {
    return publisher;
}

BookBrief& BookBrief::setPublisher(const xstring &value) {
    publisher = value;
    return *this;
}

BookBrief::xstring BookBrief::getIntroduction() const {
    return introduction;
}

BookBrief& BookBrief::setIntroduction(const xstring &value) {
    introduction = value;
    return *this;
}

BookBrief::xint BookBrief::getPriority() const {
    return priority;
}

BookBrief& BookBrief::setPriority(const xint &value) {
    priority = value;
    return *this;
}

BookBrief::xint BookBrief::getStarCount() const {
    return starCount;
}

BookBrief& BookBrief::setStarCount(const xint &value) {
    starCount = value;
    return *this;
}

BookBrief& BookBrief::setFromPtree(const ptree &pt) {
    setBookid(pt.get<xint>("bookid", 0));
    setTitle(pt.get<xstring>("title", ""));
    setAuthor(pt.get<xstring>("author", ""));
    setISBN(pt.get<xstring>("ISBN", ""));
    setPublisher(pt.get<xstring>("publisher", ""));
    setIntroduction(pt.get<xstring>("introduction", ""));
    setPriority(pt.get<xint>("priority", 0));
    setStarCount(pt.get<xint>("starCount", 0));
    return *this;
}

BookBrief BookBrief::fromPtree(const ptree &pt) {
    BookBrief book;
    book.setFromPtree(pt);
    return std::move(book);
}

const BookBrief& BookBrief::unknown() {
    static BookBrief book;
    static bool init {false};
    if (!init) {
        book.setTitle("Unknown");
        init = true;
    }
    return book;
}
