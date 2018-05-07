// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <memory>

#include <core/abstractuser.h>
#include <client/xclient.h>

class UserManager {
public:
    typedef boost::property_tree::ptree ptree;
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    bool isAdminister() const;

    void setToken(const xll &token);
    bool setUser(const ptree &pt);
    bool setLoginRecord(const ptree &pt);
    bool setBorrowRecord(const ptree &pt);
    bool setKeepRecord(const ptree &pt);
    bool setBrowseRecord(const ptree &pt);

private:
    xll token {0};
    std::shared_ptr<AbstractUser> user;
};

