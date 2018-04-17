#include "abstractuser.h"

AbstractUser::~AbstractUser() { }

X::string AbstractUser::getUserid() const {
    return userid;
}

void AbstractUser::setUserid(const X::string &value) {
    userid = value;
}

X::string AbstractUser::getUsername() const {
    return username;
}

void AbstractUser::setUsername(const X::string &value) {
    username = value;
}

X::string AbstractUser::getPassword() const {
    return password;
}

void AbstractUser::setPassword(const X::string &value) {
    password = value;
}

int AbstractUser::getPriority() const {
    return priority;
}

void AbstractUser::setPriority(int value) {
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
