// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/abstractuser.h>

AbstractUser::~AbstractUser() { }

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

AbstractUser::xstring AbstractUser::getPassword() const {
    return password;
}

void AbstractUser::setPassword(const xstring &value) {
    password = value;
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

void AbstractUser::removeLoginRecord(const LoginRecord &record) {
    loginRecordManager.remove(record);
}

void AbstractUser::clearLoginRecordManeger() {
    loginRecordManager.clear();
}

LoginRecordManager AbstractUser::getLoginRecordManager() const {
    return loginRecordManager;
}
