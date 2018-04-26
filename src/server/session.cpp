#include "session.h"

Session::Session(const X::uint &token, const X::uint &userid, const time_t &alive) :
    token(token),
    userid(userid),
    alive(alive) {

}

X::uint Session::getRandToken() {
    X::uint a = rand();
    for (; a == 0; ++a);
    return a;
}

std::time_t Session::getNowTime() {
    return time(0);
}

X::uint Session::getToken() const {
    return token;
}

void Session::setToken(const X::uint &value) {
    token = value;
}

X::uint Session::getUserid() const {
    return userid;
}

void Session::setUserid(const X::uint &value) {
    userid = value;
}

std::time_t Session::getAlive() const {
    return alive;
}

void Session::setAlive(const std::time_t &value) {
    alive = value;
}
