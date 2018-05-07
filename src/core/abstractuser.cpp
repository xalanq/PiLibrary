// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/abstractuser.h>

AbstractUser::~AbstractUser() { }

AbstractUser::xll AbstractUser::getToken() const {
    return token;
}

void AbstractUser::setToken(const xll &value) {
    token = value;
}

AbstractUser::xint AbstractUser::getUserid() const {
    return userid;
}

void AbstractUser::setUserid(const xint &value) {
    userid = value;
}

AbstractUser::xstring AbstractUser::getUsername() const {
    return username;
}

void AbstractUser::setUsername(const xstring &value) {
    username = value;
}

AbstractUser::xstring AbstractUser::getNickname() const {
    return nickname;
}

void AbstractUser::setNickname(const xstring &value) {
    nickname = value;
}

AbstractUser::xstring AbstractUser::getEmail() const {
    return email;
}

void AbstractUser::setEmail(const xstring &value) {
    email = value;
}

AbstractUser::xint AbstractUser::getPriority() const {
    return priority;
}

void AbstractUser::setPriority(const xint &value) {
    priority = value;
}

void AbstractUser::addLoginRecord(const LoginRecord &record) {
    loginRecordManager.add(record);
}

RecordManager<LoginRecord>& AbstractUser::getLoginRecordManager() {
    return loginRecordManager;
}

void AbstractUser::clearLoginRecordManeger() {
    loginRecordManager.clear();
}

