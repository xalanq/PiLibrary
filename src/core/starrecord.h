// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/xcore.h>

class StarRecord {

public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    virtual ~StarRecord();

    xint getBookid() const;
    void setBookid(const xint &value);

    xll getTime() const;
    void setTime(const xll &value);

    bool operator < (const StarRecord &b) const;

private:
    xint bookid;
    xll time;
};
