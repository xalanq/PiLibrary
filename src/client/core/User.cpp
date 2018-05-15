// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/core/User.h>

User::xint User::getUserid() const {
    return userid;
}

User& User::setUserid(const xint &value) {
    userid = value;
    return *this;
}

User::xstring User::getUsername() const {
    return username;
}

User& User::setUsername(const xstring &value) {
    username = value;
    return *this;
}

User::xstring User::getNickname() const {
    return nickname;
}

User& User::setNickname(const xstring &value) {
    nickname = value;
    return *this;
}

User::xstring User::getEmail() const {
    return email;
}

User& User::setEmail(const xstring &value) {
    email = value;
    return *this;
}

User::xint User::getPriority() const {
    return priority;
}

User& User::setPriority(const xint &value) {
    priority = value;
    return *this;
}
    
User& User::setFromPtree(const ptree &pt) {
    setUserid(pt.get<xint>("userid", 0));
    setUsername(pt.get<xstring>("username", ""));
    setNickname(pt.get<xstring>("nickname", ""));
    setEmail(pt.get<xstring>("email", ""));
    return *this;
}

User User::fromPtree(const ptree &pt) {
    User user;
    user.setFromPtree(pt);
    return std::move(user);
}
