// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <server/sessionmanager.h>

SessionManager::SessionManager(const uint &defaulAlive) :
    eng(std::random_device()()),
    defaultAlive(defaultAlive) {

}

void SessionManager::removeExpired() {
    for (; dataAlive.size(); ) {
        auto p = *dataAlive.begin();
        if (p->getAlive() < Session::getNowTime())
            remove(p);
        else
            break;
    }
}

bool SessionManager::add(const SessionManager::ptr &p) {
    removeExpired();
    auto pr = dataToken.insert(p);
    dataAlive.insert(p);
    dataUserid.insert(p);
    return pr.second;
}

bool SessionManager::add(const Session &session) {
    auto p = std::make_shared<Session> (session);
    return add(p);
}

bool SessionManager::add(const ull &token, const uint &userid, const std::time_t &alive, const uint &priority) {
    auto p = std::make_shared<Session> (token, userid, alive, priority);
    return add(p);
}

void SessionManager::remove(const SessionManager::ptr &p) {
    dataAlive.erase(p);
    dataToken.erase(p);
    dataUserid.erase(p);
}

bool SessionManager::removeByToken(const ull &token) {
    auto p = findToken(token);
    if (p == nullptr)
        return 0;
    remove(p);
    return 1;
}

bool SessionManager::removeByUserid(const uint &userid) {
    auto p = findUserid(userid);
    if (p == nullptr)
        return 0;
    remove(p);
    return 1;
}

bool SessionManager::setAliveTime(const ull &token, const time_t &alive) {
    auto p = findToken(token);
    if (p == nullptr)
        return 0;
    remove(p);
    p->setAlive(alive);
    if (alive < Session::getNowTime())
        return 0;
    add(p);
    return 1;
}

SessionManager::ptr SessionManager::findToken(const ull &token) {
    auto p = std::make_shared<Session> (token);
    removeExpired();
    auto it = dataToken.find(p);
    if (it == dataToken.end())
        return nullptr;
    return *it;
}

SessionManager::ptr SessionManager::findUserid(const uint &userid) {
    auto p = std::make_shared<Session> (0, userid);
    removeExpired();
    auto it = dataUserid.find(p);
    if (it == dataUserid.end())
        return nullptr;
    return *it;
}

SessionManager::ull SessionManager::getRandToken() {
    auto getter = [this] {
        ull token = distr(eng);
        for (; token == 0; ++token);
        return token;
    };
    ull token = getter();
    for (; findToken(token) != nullptr; token = getter());
    return token;
}

SessionManager::uint SessionManager::getDefaultAlive() const {
    return defaultAlive;
}
