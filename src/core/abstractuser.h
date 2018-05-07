// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/loginrecord.h>
#include <core/recordmanager.h>
#include <core/xcore.h>

class AbstractUser {
public:
    enum Priority {
        USER = 1,
        ADMINISTER = 50,
        SUPER_ADMINISTER = 100
    };
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    virtual ~AbstractUser() = 0;

    xll getToken() const;
    void setToken(const xll &value);

    xint getUserid() const;
    void setUserid(const xint &value);

    xstring getUsername() const;
    void setUsername(const xstring &value);

    xstring getNickname() const;
    void setNickname(const xstring &value);

    xstring getEmail() const;
    void setEmail(const xstring &value);

    xint getPriority() const;
    void setPriority(const xint &value);

    void addLoginRecord(const LoginRecord &record);
    RecordManager<LoginRecord>& getLoginRecordManager();
    void clearLoginRecordManeger();

private:
    xll token;
    xint userid;
    xstring nickname;
    xstring username;
    xstring email;
    xint priority;
    RecordManager<LoginRecord> loginRecordManager;
};
