// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <ctime>

#include <core/xcore.h>

class LoginRecord {
public:
    typedef X::uint uint;
    typedef X::ull ull;
    typedef X::string string;

    string getIp() const;
    void setIp(const string &value);

    std::time_t getTime() const;
    void setTime(const std::time_t &value);

    bool operator < (const LoginRecord &b) const;

private:
    string ip;
    std::time_t time;
};
