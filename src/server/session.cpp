#include "session.h"

Session::Session(const ull &token, const uint &userid, const time_t &alive, const uint &priority) :
    token(token),
    userid(userid),
    alive(alive),
    priority(priority) {

}

std::time_t Session::getNowTime() {
    return time(0);
}

Session::ull Session::getToken() const {
    return token;
}

void Session::setToken(const ull &value) {
    token = value;
}

Session::uint Session::getUserid() const {
    return userid;
}

void Session::setUserid(const uint &value) {
    userid = value;
}

std::time_t Session::getAlive() const {
    return alive;
}

void Session::setAlive(const std::time_t &value) {
    alive = value;
}

Session::uint Session::getPriority() const {
    return priority;
}

void Session::setPriority(const uint &value) {
    priority = value;
}
