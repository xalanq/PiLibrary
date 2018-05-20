// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <set>

#include <client/core/User.h>
#include <core/types.h>

class UserManager {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;

    bool isAdminister() const;

    xll getToken() const;
    void setToken(const xll &token);
    
    User& getUser();
    void setUser(const ptree &pt);

    std::set<xint>& getStarBooks();
    std::set<xint>& getKeepBooks();

private:
    xll token {};
    User user {};
    std::set<xint> starBooks {};
    std::set<xint> keepBooks {};
};

