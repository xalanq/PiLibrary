// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <ctime>

#include <core/xcore.h>

class BorrowRecord {
public:
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;

    uint getBookid() const;
    void setBookid(const uint &value);

    void setTime(const std::time_t &beginTime, const std::time_t &endTime);

    std::time_t getBeginTime() const;
    void setBeginTime(const std::time_t &value);

    std::time_t getEndTime() const;
    void setEndTime(const std::time_t &value);
    
    bool operator < (const BorrowRecord &t) const;

private:
    uint bookid;
    std::time_t beginTime;
    std::time_t endTime;
};
