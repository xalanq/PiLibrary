// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <map>

#include <core/book.h>
#include <client/xclient.h>

class BookManager {

public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    static Book parseBook(const ptree &pt);

    bool add(const Book &book, bool force = false);

    bool add(const ptree &pt, bool force = false);

    bool has(const xint &bookid) const;

    const Book& get(const xint &bookid) const;

private:
    std::map<xint, Book> books;
};