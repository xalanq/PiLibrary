// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/xcore.h>

class BrowseRecord {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;
    
    xint getBookid() const;
    void setBookid(const xint &value);

    xll getTime() const;
    void setTime(const xll &value);

    static BrowseRecord fromPtree(const ptree &pt);

    bool operator < (const BrowseRecord &a) const;

private:
    xint bookid;
    xll time;
};
