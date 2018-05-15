// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/types.h>

class BrowseRecord {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;
    
    xint getBookid() const;
    BrowseRecord& setBookid(const xint &value);

    xll getTime() const;
    BrowseRecord& setTime(const xll &value);

    virtual BrowseRecord& setFromPtree(const ptree &pt);
    static BrowseRecord fromPtree(const ptree &pt);

    bool operator < (const BrowseRecord &a) const;

private:
    xint bookid{};
    xll time{};
};
