// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/manager/UserManager.h>

bool UserManager::isAdminister() const {
    if (user)
        return user->getPriority() == X::ADMINISTER;
    return 0;
}

UserManager::xll UserManager::getToken() const {
    return token;
}

std::shared_ptr<User> UserManager::getUser() {
    return user;
}

void UserManager::setToken(const xll &token) {
    this->token = token;
}

void UserManager::setUser(const ptree &pt) {
    user = std::make_shared<User>();
    user->fromPtree(pt);
}
