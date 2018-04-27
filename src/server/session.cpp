#include "session.h"

Session::Session(const X::ull &token, const X::uint &userid, const time_t &alive) :
    token(token),
    userid(userid),
    alive(alive) {

}

std::time_t Session::getNowTime() {
    return time(0);
}

X::ull Session::getToken() const {
    return token;
}

void Session::setToken(const X::ull &value) {
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
