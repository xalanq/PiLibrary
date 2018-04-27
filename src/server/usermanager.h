#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "xserver.h"
#include "../core/abstractuser.h"

class UserManager {
public:
    UserManager(const X::string &sql_url, const int &sql_port);
    ~UserManager();

    bool isUser(const X::uint &userid) const;
    int getPriority(const int &userid) const;
    void registerUser(const X::string &username, const X::string &password, const int &priority);
};

#endif // USERMANAGER_H
