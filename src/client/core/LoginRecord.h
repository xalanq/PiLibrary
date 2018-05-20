// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/types.h>

class LoginRecord {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;

    xstring getIp() const;
    LoginRecord& setIp(const xstring &value);

    xll getTime() const;
    LoginRecord& setTime(const xll &value);

    virtual LoginRecord& setFromPtree(const ptree &pt);
    static LoginRecord fromPtree(const ptree &pt);

    bool operator < (const LoginRecord &b) const;

private:
    xstring ip {};
    xll time {};
};
