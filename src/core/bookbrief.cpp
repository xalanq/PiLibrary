// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/bookbrief.h>

BookBrief::~BookBrief() { }

BookBrief::xint BookBrief::getBookid() const {
    return bookid;
}

void BookBrief::setBookid(const xint &value) {
    bookid = value;
}

BookBrief::xstring BookBrief::getTitle() const {
    return title;
}

void BookBrief::setTitle(const xstring &value) {
    title = value;
}

BookBrief::xstring BookBrief::getAuthor() const {
    return author;
}

void BookBrief::setAuthor(const xstring &value) {
    author = value;
}

BookBrief::xstring BookBrief::getISBN() const {
    return ISBN;
}

void BookBrief::setISBN(const xstring &value) {
    ISBN = value;
}

BookBrief::xstring BookBrief::getPublisher() const {
    return publisher;
}

void BookBrief::setPublisher(const xstring &value) {
    publisher = value;
}

BookBrief::xstring BookBrief::getIntroduction() const {
    return introduction;
}

void BookBrief::setIntroduction(const xstring &value) {
    introduction = value;
}

BookBrief::xint BookBrief::getPriority() const {
    return priority;
}

void BookBrief::setPriority(const xint &value) {
    priority = value;
}
