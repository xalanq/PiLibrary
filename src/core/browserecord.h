// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <ctime>

#include <core/xcore.h>

class BrowseRecord {
public:
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;
    
    uint getBookid() const;
    void setBookid(const uint &value);

    std::time_t getTime() const;
    void setTime(const std::time_t &value);

    bool operator < (const BrowseRecord &a) const;

private:
    uint bookid;
    std::time_t time;
};
