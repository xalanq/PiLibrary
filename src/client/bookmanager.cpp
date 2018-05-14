// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <memory>

#include <client/bookmanager.h>

Book BookManager::parseBook(const ptree &pt) {
    Book book;
    book.setBookid(pt.get<xint>("bookid", 0));
    book.setTitle(pt.get<xstring>("title", ""));
    book.setAuthor(pt.get<xstring>("author", ""));
    book.setISBN(pt.get<xstring>("ISBN", ""));
    book.setPublisher(pt.get<xstring>("publisher", ""));
    book.setAmount(pt.get<xint>("amount", 0));
    book.setIntroduction(pt.get<xstring>("introduction", ""));
    book.setPosition(pt.get<xstring>("position", ""));
    book.setPriority(pt.get<xint>("priority", 0));
    book.setStarCount(pt.get<xint>("starCount", 0));
    return std::move(book);
}

bool BookManager::add(const Book &book, bool force) {
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

bool BookManager::add(const ptree &pt, bool force) {
    auto bookid = pt.get<xint>("bookid", 0);
    if (bookid > 0) {
        auto it = books.find(bookid);
        if (it != books.end() && !force)
            return false;
        books[bookid] = parseBook(pt);
        return true;
    }
    return false;
}

bool BookManager::has(const xint &bookid) const {
    return books.find(bookid) != books.end();
}

const Book& BookManager::get(const xint &bookid) const {
    try {
        return books.at(bookid);
    } catch (std::exception &) {
        static Book book;
        book.setTitle("Unknown book");
        return book;
    }
}
