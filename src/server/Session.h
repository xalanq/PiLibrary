// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/types.h>

class Session {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    Session(const xll &token = 0, const xint &userid = 0, const xll &alive = 0, const xint &priority = 0);

    static xll getNowTime();

    xll getToken() const;
    void setToken(const xll &value);

    xint getUserid() const;
    void setUserid(const xint &value);

    xll getAlive() const;
    void setAlive(const xll &value);

    xint getPriority() const;
    void setPriority(const xint &value);

private:
    xll token;
    xint userid;
    xll alive;
    xint priority;
};
