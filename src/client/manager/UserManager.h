// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <functional>
#include <set>
#include <vector>

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

    void starBook(const xint &bookid);
    void unStarBook(const xint &bookid);
    bool isStaredBook(const xint &bookid);
    void clearStar();
    void installStarEvent(std::function<void(xint &, bool)> f);

    void borrowBook(const xint &bookid, const xll &beginTime, const xll &endTime);
    bool isBorrowedBook(const xint &bookid);
    void clearBorrowBook();
    void installBorrowEvent(std::function<void(xint &, xll &, xll &)> f);

    void refresh();

private:
    void emitStarEvents(const xint &bookid, bool star);
    void emitBorrowEvents(const xint &bookid, const xll &beginTime, const xll &endTime);
    
private:
    xll token {};
    User user {};
    std::set<xint> starBooks {};
    std::set<xint> borrowBooks {};
    std::vector<std::function<void(xint &, bool)>> starEvents {};
    std::vector<std::function<void(xint &, xll &, xll &)>> borrowEvents {};
};

