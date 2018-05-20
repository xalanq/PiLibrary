// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/types.h>

class StarRecord {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;

    virtual ~StarRecord();

    xint getBookid() const;
    StarRecord& setBookid(const xint &value);

    xll getTime() const;
    StarRecord& setTime(const xll &value);

    virtual StarRecord& setFromPtree(const ptree &pt);
    static StarRecord fromPtree(const ptree &pt);

    bool operator < (const StarRecord &b) const;

private:
    xint bookid {};
    xll time {};
};
