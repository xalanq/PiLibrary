// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/types.h>

class KeepRecord {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;

    virtual ~KeepRecord();

    xint getBookid() const;
    KeepRecord& setBookid(const xint &value);

    xll getBeginTime() const;
    KeepRecord& setBeginTime(const xll &value);

    xll getEndTime() const;
    KeepRecord& setEndTime(const xll &value);

    KeepRecord& setTime(const xll &beginTime, const xll &endTime);

    virtual KeepRecord& setFromPtree(const ptree &pt);
    static KeepRecord fromPtree(const ptree &pt);
    
    bool operator < (const KeepRecord &t) const;

private:
    xint bookid {};
    xll beginTime {};
    xll endTime {};
};
