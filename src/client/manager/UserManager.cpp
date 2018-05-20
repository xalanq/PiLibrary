// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/manager/UserManager.h>

bool UserManager::isAdminister() const {
    return user.getPriority() == X::ADMINISTER;
}

UserManager::xll UserManager::getToken() const {
    return token;
}

User& UserManager::getUser() {
    return user;
}

void UserManager::setToken(const xll &token) {
    this->token = token;
}

void UserManager::setUser(const ptree &pt) {
    user.setFromPtree(pt);
}

std::set<UserManager::xint>& UserManager::getStarBooks() {
    return starBooks;
}

std::set<UserManager::xint>& UserManager::getKeepBooks() {
    return keepBooks;
}
