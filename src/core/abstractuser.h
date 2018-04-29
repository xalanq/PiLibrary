#ifndef ABSTRACTUSER_H
#define ABSTRACTUSER_H

#include "xcore.h"
#include "loginrecordmanager.h"

class AbstractUser {
public:
    enum Priority {
        User = 1,
        Administer = 50,
        SuperAdminister = 100
    };
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;

    virtual ~AbstractUser() = 0;

    uint getUserid() const;
    void setUserid(const uint &value);

    string getUsername() const;
    void setUsername(const string &value);

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
    string username;
    string password;
    uint priority;
    LoginRecordManager loginRecordManager;
};

#endif // ABSTRACTUSER_H
