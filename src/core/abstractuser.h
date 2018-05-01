// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/loginrecordmanager.h>
#include <core/xcore.h>

class AbstractUser {
public:
    enum Priority {
        USER = 1,
        ADMINISTER = 50,
        SUPER_ADMINISTER = 100
    };
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;

    virtual ~AbstractUser() = 0;

    uint getUserid() const;
    void setUserid(const uint &value);

    string getUsername() const;
    void setUsername(const string &value);

    string getNickname() const;
    void setNickname(const string &value);

    string getPassword() const;
    void setPassword(const string &value);

    uint getPriority() const;
    void setPriority(const uint &value);

    void addLoginRecord(const LoginRecord &record);
    void removeLoginRecord(const LoginRecord &record);
    void clearLoginRecordManeger();
    LoginRecordManager getLoginRecordManager() const;

private:
    uint userid;
    string nickname;
    string username;
    string password;
    uint priority;
    LoginRecordManager loginRecordManager;
};
