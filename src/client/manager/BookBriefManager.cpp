// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <memory>

#include <client/manager/BookBriefManager.h>

bool BookBriefManager::add(const BookBrief &book, bool force) {
    auto bookid = book.getBookid();
    if (book.getBookid() > 0) {
        auto it = books.find(bookid);
        if (it != books.end() && !force)
            return false;
        books[bookid] = book;
        return true;
    }
    return false;
}

bool BookBriefManager::add(const ptree &pt, bool force) {
    auto bookid = pt.get<xint>("bookid", 0);
    if (bookid > 0) {
        auto it = books.find(bookid);
        if (it != books.end() && !force)
            return false;
        books[bookid] = BookBrief::fromPtree(pt);
        return true;
    }
    return false;
}

bool BookBriefManager::has(const xint &bookid) const {
    return books.find(bookid) != books.end();
}

const BookBrief& BookBriefManager::get(const xint &bookid) const {
    try {
        return books.at(bookid);
    } catch (std::exception &) {
        static BookBrief book;
        book.setTitle("Unknown book");
        return book;
    }
}
