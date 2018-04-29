#include "bookcore.h"

BookCore::~BookCore() { }

BookCore::uint BookCore::getBookid() const {
    return bookid;
}

void BookCore::setBookid(const uint &value) {
    bookid = value;
}

BookCore::string BookCore::getTitle() const {
    return title;
}

void BookCore::setTitle(const string &value) {
    title = value;
}

BookCore::string BookCore::getAuthor() const {
    return author;
}

void BookCore::setAuthor(const string &value) {
    author = value;
}

BookCore::string BookCore::getISBN() const {
    return ISBN;
}

void BookCore::setISBN(const string &value) {
    ISBN = value;
}

BookCore::string BookCore::getPublisher() const {
    return publisher;
}

void BookCore::setPublisher(const string &value) {
    publisher = value;
}

BookCore::uint BookCore::getAmount() const {
    return amount;
}

void BookCore::setAmount(const uint &value) {
    amount = value;
}

BookCore::string BookCore::getIntroduction() const {
    return introduction;
}

void BookCore::setIntroduction(const string &value) {
    introduction = value;
}

BookCore::uint BookCore::getPriority() const {
    return priority;
}

void BookCore::setPriority(const uint &value) {
    priority = value;
}
