// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <memory>

#include <core/abstractuser.h>
#include <core/borrowrecord.h>
#include <core/browserecord.h>
#include <core/loginrecord.h>
#include <core/recordmanager.h>
#include <core/starrecord.h>
#include <client/xclient.h>

class UserManager {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    bool isAdminister() const;

    xll getToken() const;
    std::shared_ptr<AbstractUser> getUser();
    RecordManager<LoginRecord>* getLoginRecordManager();
    RecordManager<StarRecord>* getStarRecordManager();
    RecordManager<BorrowRecord>* getBorrowRecordManager();
    RecordManager<BorrowRecord>* getKeepRecordManager();
    RecordManager<BrowseRecord>* getBrowseRecordManager();

    void setToken(const xll &token);
    bool setUser(const ptree &pt);
    bool setLoginRecord(const ptree &pt);
    bool setStarRecord(const ptree &pt);
    bool setBorrowRecord(const ptree &pt);
    bool setKeepRecord(const ptree &pt);
    bool setBrowseRecord(const ptree &pt);

private:
    xll token {0};
    std::shared_ptr<AbstractUser> user;
};

