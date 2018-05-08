// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/administer.h>
#include <core/borrowrecord.h>
#include <core/browserecord.h>
#include <core/loginrecord.h>
#include <core/user.h>
#include <client/usermanager.h>

bool UserManager::isAdminister() const {
    auto it = std::dynamic_pointer_cast<Administer> (user);
    return bool(it);
}

UserManager::xll UserManager::getToken() const {
    return token;
}

std::shared_ptr<AbstractUser> UserManager::getUser() {
    return user;
}

RecordManager<LoginRecord>* UserManager::getLoginRecordManager() {
    if (user)
        return &user->getLoginRecordManager();
    return nullptr;
}

RecordManager<BorrowRecord>* UserManager::getBorrowRecordManager() {
    auto it = std::dynamic_pointer_cast<User>(user);
    if (it)
        return &it->getBorrowRecordManager();
    return nullptr;
}

RecordManager<BorrowRecord>* UserManager::getKeepRecordManager() {
    auto it = std::dynamic_pointer_cast<User>(user);
    if (it)
        return &it->getKeepRecordManager();
    return nullptr;
}

RecordManager<BrowseRecord>* UserManager::getBrowseRecordManager() {
    auto it = std::dynamic_pointer_cast<User>(user);
    if (it)
        return &it->getBrowseRecordManager();
    return nullptr;
}

void UserManager::setToken(const xll &token) {
    this->token = token;
}

bool UserManager::setUser(const ptree &pt) {
    auto priority = pt.get<xint>("priority", 0);
    if (priority == AbstractUser::USER)
        user = std::make_shared<User>();
    else if (priority == AbstractUser::ADMINISTER)
        user = std::make_shared<Administer>();
    else
        return false;
    user->setUserid(pt.get<xint>("userid", 0));
    user->setUsername(pt.get<xstring>("username", ""));
    user->setPriority(priority);
    user->setNickname(pt.get<xstring>("nickname", ""));
    user->setEmail(pt.get<xstring>("email", ""));
    return true;
}

bool UserManager::setLoginRecord(const ptree &pt) {
    auto x = pt.get_child_optional("loginRecord");
    if (x && user) {
        auto it = user;
        for (auto &item: *x) {
            LoginRecord record;
            auto ip = item.second.get_optional<xstring>("ip");
            if (!ip)
                return false;
            auto time = item.second.get_optional<xll>("time");
            if (!time)
                return false;
            record.setIp(*ip);
            record.setTime(*time);
            it->addLoginRecord(record);
        }
        return true;
    }
    return false;
}

bool UserManager::setBorrowRecord(const ptree &pt) {
    auto x = pt.get_child_optional("borrowRecord");
    if (x && user) {
        auto it = std::dynamic_pointer_cast<User>(user);
        for (auto &item: *x) {
            BorrowRecord record;
            auto bookid = item.second.get_optional<xint>("bookid");
            if (!bookid)
                return false;
            auto beginTime = item.second.get_optional<xll>("beginTime");
            if (!beginTime)
                return false;
            auto endTime = item.second.get_optional<xll>("endTime");
            if (!endTime)
                return false;
            auto returnTime = item.second.get_optional<xll>("returnTime");
            if (!returnTime)
                return false;
            record.setBookid(*bookid);
            record.setTime(*beginTime, *endTime, *returnTime);
            it->addBorrowRecord(record);
        }
        return true;
    }
    return false;
}

bool UserManager::setKeepRecord(const ptree &pt) {
    auto x = pt.get_child_optional("keepRecord");
    if (x && user) {
        auto it = std::dynamic_pointer_cast<User>(user);
        for (auto &item: *x) {
            BorrowRecord record;
            auto bookid = item.second.get_optional<xint>("bookid");
            if (!bookid)
                return false;
            auto beginTime = item.second.get_optional<xll>("beginTime");
            if (!beginTime)
                return false;
            auto endTime = item.second.get_optional<xll>("endTime");
            if (!endTime)
                return false;
            auto returnTime = item.second.get_optional<xll>("returnTime");
            if (!returnTime)
                return false;
            record.setBookid(*bookid);
            record.setTime(*beginTime, *endTime, *returnTime);
            it->addBorrowRecord(record);
        }
        return true;
    }
    return false;
}

bool UserManager::setBrowseRecord(const ptree &pt) {
    auto x = pt.get_child_optional("browseRecord");
    if (x && user) {
        auto it = std::dynamic_pointer_cast<User>(user);
        for (auto &item: *x) {
            BrowseRecord record;
            auto bookid = item.second.get_optional<xint>("bookid");
            if (!bookid)
                return false;
            auto time = item.second.get_optional<xll>("time");
            if (!time)
                return false;
            record.setBookid(*bookid);
            record.setTime(*time);
            it->addBrowseRecord(record);
        }
        return true;
    }
    return false;
}
