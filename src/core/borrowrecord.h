// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <ctime>

#include <core/xcore.h>

class BorrowRecord {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    xint getBookid() const;
    void setBookid(const xint &value);

    void setTime(const std::time_t &beginTime, const std::time_t &endTime);

    std::time_t getBeginTime() const;
    void setBeginTime(const std::time_t &value);

    std::time_t getEndTime() const;
    void setEndTime(const std::time_t &value);
    
    bool operator < (const BorrowRecord &t) const;

private:
    xint bookid;
    std::time_t beginTime;
    std::time_t endTime;
};
