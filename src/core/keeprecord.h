// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/xcore.h>

class KeepRecord {

public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    virtual ~KeepRecord();

    xint getBookid() const;
    void setBookid(const xint &value);

    void setTime(const xll &beginTime, const xll &endTime);

    xll getBeginTime() const;
    void setBeginTime(const xll &value);

    xll getEndTime() const;
    void setEndTime(const xll &value);

    static KeepRecord fromPtree(const ptree &pt);
    
    bool operator < (const KeepRecord &t) const;

private:
    xint bookid;
    xll beginTime;
    xll endTime;
};
