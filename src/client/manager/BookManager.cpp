// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <memory>

#include <client/manager/BookManager.h>

bool BookManager::addBookBrief(const BookBrief &book, bool force) {
    auto bookid = book.getBookid();
    if (book.getBookid() > 0) {
        auto it = mapBookBrief.find(bookid);
        if (it != mapBookBrief.end() && !force)
            return false;
        mapBookBrief[bookid] = book;
        return true;
    }
    return false;
}

bool BookManager::addBookBrief(const ptree &pt, bool force) {
    auto bookid = pt.get<xint>("bookid", 0);
    if (bookid > 0) {
        auto it = mapBookBrief.find(bookid);
        if (it != mapBookBrief.end() && !force)
            return false;
        mapBookBrief[bookid] = BookBrief::fromPtree(pt);
        return true;
    }
    return false;
}


bool BookManager::addBook(const Book &book, bool force) {
    auto bookid = book.getBookid();
    if (book.getBookid() > 0) {
        auto it = mapBook.find(bookid);
        if (it != mapBook.end() && !force)
            return false;
        mapBook[bookid] = book;
        return true;
    }
    return false;

}

bool BookManager::addBook(const ptree &pt, bool force) {
    auto bookid = pt.get<xint>("bookid", 0);
    if (bookid > 0) {
        auto it = mapBook.find(bookid);
        if (it != mapBook.end() && !force)
            return false;
        mapBook[bookid] = Book::fromPtree(pt);
        return true;
    }
    return false;
}

bool BookManager::hasBookBrief(const xint &bookid) const {
    return mapBookBrief.find(bookid) != mapBookBrief.end();
}

const BookBrief& BookManager::getBookBrief(const xint &bookid) const {
    try {
        return mapBookBrief.at(bookid);
    } catch (std::exception &) {
        static BookBrief book;
        book.setTitle("Unknown book");
        return book;
    }
}
