// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <memory>

#include <client/core/User.h>
#include <client/core/BorrowRecord.h>
#include <client/core/BrowseRecord.h>
#include <client/core/KeepRecord.h>
#include <client/core/LoginRecord.h>
#include <client/core/StarRecord.h>
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
    
    std::shared_ptr<User> getUser();
    void setUser(const ptree &pt);

private:
    xll token {};
    std::shared_ptr<User> user;
};

