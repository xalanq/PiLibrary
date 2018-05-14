// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/loginrecord.h>

LoginRecord::xstring LoginRecord::getIp() const {
    return ip;
}

void LoginRecord::setIp(const xstring &value) {
    ip = value;
}

LoginRecord::xll LoginRecord::getTime() const {
    return time;
}

void LoginRecord::setTime(const xll &value) {
    time = value;
}

LoginRecord LoginRecord::fromPtree(const ptree &pt) {
    LoginRecord record;
    record.setIp(pt.get<xstring>("ip", ""));
    record.setTime(pt.get<xll>("time", 0));
    return std::move(record);
}

bool LoginRecord::operator < (const LoginRecord &b) const {
    return time == b.time ? ip < b.ip : time < b.time;
}
