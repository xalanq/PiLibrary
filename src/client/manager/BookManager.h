// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <map>

#include <client/core/Book.h>
#include <client/core/BookBrief.h>
#include <core/types.h>

class BookManager {

public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;

    bool addBookBrief(const BookBrief &book, bool force = false);

    bool addBookBrief(const ptree &pt, bool force = false);

    bool addBook(const Book &book, bool force = false);

    bool addBook(const ptree &pt, bool force = false);

    bool hasBookBrief(const xint &bookid) const;

    const BookBrief& getBookBrief(const xint &bookid) const;

private:
    std::map<xint, Book> mapBook;
    std::map<xint, BookBrief> mapBookBrief;
};