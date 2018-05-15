// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/types.h>

class BorrowRecord {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;

    virtual ~BorrowRecord();

    xint getBookid() const;
    BorrowRecord& setBookid(const xint &value);

    xll getBeginTime() const;
    BorrowRecord& setBeginTime(const xll &value);

    xll getEndTime() const;
    BorrowRecord& setEndTime(const xll &value);

    xll getReturnTime() const;
    BorrowRecord& setReturnTime(const xll &value);

    BorrowRecord& setTime(const xll &beginTime, const xll &endTime, const xll &returnTime);

    virtual BorrowRecord& setFromPtree(const ptree &pt);
    static BorrowRecord fromPtree(const ptree &pt);
    
    bool operator < (const BorrowRecord &t) const;

private:
    xint bookid{};
    xll beginTime{};
    xll endTime{};
    xll returnTime{};
};
