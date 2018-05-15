// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/core/LoginRecord.h>

LoginRecord::xstring LoginRecord::getIp() const {
    return ip;
}

LoginRecord& LoginRecord::setIp(const xstring &value) {
    ip = value;
    return *this;
}

LoginRecord::xll LoginRecord::getTime() const {
    return time;
}

LoginRecord& LoginRecord::setTime(const xll &value) {
    time = value;
    return *this;
}

LoginRecord& LoginRecord::setFromPtree(const ptree &pt) {
    setIp(pt.get<xstring>("ip", ""));
    setTime(pt.get<xll>("time", 0));
    return *this;
}

LoginRecord LoginRecord::fromPtree(const ptree &pt) {
    LoginRecord record;
    record.setFromPtree(pt);
    return std::move(record);
}

bool LoginRecord::operator < (const LoginRecord &b) const {
    return time == b.time ? ip < b.ip : time < b.time;
}
