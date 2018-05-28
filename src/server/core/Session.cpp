// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <ctime>

#include <server/core/Session.h>

Session::Session(const xll &token, const xint &userid, const xll &alive, const xint &priority) :
    token(token),
    userid(userid),
    alive(alive),
    priority(priority) {

}

Session::xll Session::getNowTime() {
    return time(0);
}

Session::xll Session::getToken() const {
    return token;
}

void Session::setToken(const xll &value) {
    token = value;
}

Session::xint Session::getUserid() const {
    return userid;
}

void Session::setUserid(const xint &value) {
    userid = value;
}

Session::xll Session::getAlive() const {
    return alive;
}

void Session::setAlive(const xll &value) {
    alive = value;
}

Session::xint Session::getPriority() const {
    return priority;
}

void Session::setPriority(const xint &value) {
    priority = value;
}
