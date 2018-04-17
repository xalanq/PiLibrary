#ifndef ABSTRACTUSER_H
#define ABSTRACTUSER_H

#include "loginhistorymanager.h"
#include <string>

class AbstractUser {
public:
    AbstractUser();
    virtual ~AbstractUser() = 0;

private:
    std::string userid;
    std::string username;
    std::string password;
    int priority;
    LoginHistoryManager loginHistory;
};

#endif // ABSTRACTUSER_H
