// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/bookcore.h>

BookCore::~BookCore() { }

BookCore::xint BookCore::getBookid() const {
    return bookid;
}

void BookCore::setBookid(const xint &value) {
    bookid = value;
}

BookCore::xstring BookCore::getTitle() const {
    return title;
}

void BookCore::setTitle(const xstring &value) {
    title = value;
}

BookCore::xstring BookCore::getAuthor() const {
    return author;
}

void BookCore::setAuthor(const xstring &value) {
    author = value;
}

BookCore::xstring BookCore::getISBN() const {
    return ISBN;
}

void BookCore::setISBN(const xstring &value) {
    ISBN = value;
}

BookCore::xstring BookCore::getPublisher() const {
    return publisher;
}

void BookCore::setPublisher(const xstring &value) {
    publisher = value;
}

BookCore::xint BookCore::getAmount() const {
    return amount;
}

void BookCore::setAmount(const xint &value) {
    amount = value;
}

BookCore::xstring BookCore::getIntroduction() const {
    return introduction;
}

void BookCore::setIntroduction(const xstring &value) {
    introduction = value;
}

BookCore::xstring BookCore::getPosition() const {
    return position;
}

void BookCore::setPosition(const xstring &value) {
    position = value;
}

BookCore::xint BookCore::getPriority() const {
    return priority;
}

void BookCore::setPriority(const xint &value) {
    priority = value;
}
