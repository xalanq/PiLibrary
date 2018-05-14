// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/xcore.h>

class BorrowRecord {

public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    virtual ~BorrowRecord();

    xint getBookid() const;
    void setBookid(const xint &value);

    void setTime(const xll &beginTime, const xll &endTime, const xll &returnTime);

    xll getBeginTime() const;
    void setBeginTime(const xll &value);

    xll getEndTime() const;
    void setEndTime(const xll &value);

    xll getReturnTime() const;
    void setReturnTime(const xll &value);
    
    bool operator < (const BorrowRecord &t) const;

private:
    xint bookid;
    xll beginTime;
    xll endTime;
    xll returnTime;
};
