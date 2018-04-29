#include "abstractuser.h"

AbstractUser::~AbstractUser() { }

AbstractUser::uint AbstractUser::getUserid() const {
    return userid;
}

void AbstractUser::setUserid(const uint &value) {
    userid = value;
}

AbstractUser::string AbstractUser::getUsername() const {
    return username;
}

void AbstractUser::setUsername(const string &value) {
    username = value;
}

AbstractUser::string AbstractUser::getPassword() const {
    return password;
}

void AbstractUser::setPassword(const string &value) {
    password = value;
}

AbstractUser::uint AbstractUser::getPriority() const {
    return priority;
}

void AbstractUser::setPriority(const uint &value) {
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
