// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/manager/UserManager.h>

bool UserManager::isAdminister() const {
    return user.getPriority() >= X::ADMINISTER;
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

void UserManager::starBook(const xint &bookid) {
    starBooks.insert(bookid);
    emitStarEvents(bookid, true);
}

void UserManager::unStarBook(const xint &bookid) {
    starBooks.erase(bookid);
    emitStarEvents(bookid, false);
}

bool UserManager::isStaredBook(const xint &bookid) {
    return starBooks.find(bookid) != starBooks.end();
}

void UserManager::clearStar() {
    starBooks.clear();
    starEvents.clear();
}

void UserManager::installStarEvent(std::function<void(xint &, bool)> f) {
    starEvents.push_back(f);
}

void UserManager::clearStarEvent() {
    starEvents.clear();
}

void UserManager::borrowBook(const xint &bookid, const xll &beginTime, const xll &endTime) {
    borrowBooks.insert(bookid);
    emitBorrowEvents(bookid, beginTime, endTime);
}

bool UserManager::isBorrowedBook(const xint &bookid) {
    return borrowBooks.find(bookid) != borrowBooks.end();
}

void UserManager::clearBorrowBook() {
    borrowBooks.clear();
}

void UserManager::installBorrowEvent(std::function<void(xint &, xll &, xll &)> f) {
    borrowEvents.push_back(f);
}

void UserManager::clearBorrowEvent() {
    borrowEvents.clear();
}

void UserManager::refresh() {
    starBooks.clear();
    starEvents.clear();
    borrowBooks.clear();
    borrowEvents.clear();
}

void UserManager::emitStarEvents(const xint &bookid, bool star) {
    for (auto f: starEvents)
        f(xint(bookid), star);
}

void UserManager::emitBorrowEvents(const xint &bookid, const xll &beginTime, const xll &endTime) {
    for (auto f: borrowEvents)
        f(xint(bookid), xll(beginTime), xll(endTime));
}
