#ifndef ABSTRACTUSER_H
#define ABSTRACTUSER_H

#include "xcore.h"
#include "loginrecordmanager.h"

class AbstractUser {
public:
    virtual ~AbstractUser() = 0;

    X::string getUserid() const;
    void setUserid(const X::string &value);

    X::string getUsername() const;
    void setUsername(const X::string &value);

    X::string getPassword() const;
    void setPassword(const X::string &value);

    int getPriority() const;
    void setPriority(int value);

    void addLoginRecord(const LoginRecord &record);
    void removeLoginRecord(const LoginRecord &record);
    void clearLoginRecordManeger();
    LoginRecordManager getLoginRecordManager() const;

private:
    X::string userid;
    X::string username;
    X::string password;
    int priority;
    LoginRecordManager loginRecordManager;
};

#endif // ABSTRACTUSER_H
