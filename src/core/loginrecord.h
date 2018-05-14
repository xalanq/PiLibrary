// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/xcore.h>

class LoginRecord {
public:
    typedef X::xint xint;
    typedef X::xll xll;
    typedef X::xstring xstring;

    xstring getIp() const;
    void setIp(const xstring &value);

    xll getTime() const;
    void setTime(const xll &value);

    static LoginRecord fromPtree(const ptree &pt);

    bool operator < (const LoginRecord &b) const;

private:
    xstring ip;
    xll time;
};
