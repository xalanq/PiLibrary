#include "bookcore.h"

BookCore::~BookCore() { }

X::uint BookCore::getBookid() const {
    return bookid;
}

void BookCore::setBookid(const X::uint &value) {
    bookid = value;
}

X::string BookCore::getTitle() const {
    return title;
}

void BookCore::setTitle(const X::string &value) {
    title = value;
}

X::string BookCore::getAuthor() const {
    return author;
}

void BookCore::setAuthor(const X::string &value) {
    author = value;
}

X::string BookCore::getISBN() const {
    return ISBN;
}

void BookCore::setISBN(const X::string &value) {
    ISBN = value;
}

X::string BookCore::getPublisher() const {
    return publisher;
}

void BookCore::setPublisher(const X::string &value) {
    publisher = value;
}

X::uint BookCore::getAmount() const {
    return amount;
}

void BookCore::setAmount(const X::uint &value) {
    amount = value;
}

X::string BookCore::getIntroduction() const {
    return introduction;
}

void BookCore::setIntroduction(const X::string &value) {
    introduction = value;
}

int BookCore::getPriority() const {
    return priority;
}

void BookCore::setPriority(int value) {
    priority = value;
}
