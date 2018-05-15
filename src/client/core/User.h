// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/types.h>

class User {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;

    virtual ~User() = default;

    xint getUserid() const;
    User& setUserid(const xint &value);

    xstring getUsername() const;
    User& setUsername(const xstring &value);

    xstring getNickname() const;
    User& setNickname(const xstring &value);

    xstring getEmail() const;
    User& setEmail(const xstring &value);

    xint getPriority() const;
    User& setPriority(const xint &value);
    
    virtual User& setFromPtree(const ptree &pt);
    static User fromPtree(const ptree &pt);

private:
    xint userid{};
    xstring username{};
    xstring nickname{};
    xstring email{};
    xint priority{};
};
