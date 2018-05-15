// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <map>

#include <client/core/BookBrief.h>
#include <core/types.h>

class BookBriefManager {

public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;

    bool add(const BookBrief &book, bool force = false);

    bool add(const ptree &pt, bool force = false);

    bool has(const xint &bookid) const;

    const BookBrief& get(const xint &bookid) const;

private:
    std::map<xint, BookBrief> books;
};