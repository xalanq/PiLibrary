#include "loginrecord.h"

LoginRecord::string LoginRecord::getIp() const {
    return ip;
}

void LoginRecord::setIp(const string &value) {
    ip = value;
}

std::time_t LoginRecord::getTime() const {
    return time;
}

void LoginRecord::setTime(const std::time_t &value) {
    time = value;
}

bool LoginRecord::operator < (const LoginRecord &b) const {
    return time == b.time ? ip < b.ip : time < b.time;
}
