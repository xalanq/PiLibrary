// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <server/SessionManager.h>

SessionManager::SessionManager(const xll &defaultAlive) :
    eng(std::random_device()()),
    defaultAlive(defaultAlive) {
}

void SessionManager::removeExpired() {
    boost::upgrade_lock<boost::shared_mutex> _lock(_access);
    boost::upgrade_to_unique_lock<boost::shared_mutex> _uniqueLock(_lock);
    for (; dataAlive.size(); ) {
        auto p = *dataAlive.begin();
        if (p->getAlive() < Session::getNowTime())
            remove(p);
        else
            break;
    }
}

bool SessionManager::add(const SessionManager::ptr &p, bool force) {
    removeExpired();
    if (force) {
        auto it = findUserid(p->getUserid());
        if (it != nullptr)
            remove(it);
    }
    boost::upgrade_lock<boost::shared_mutex> _lock(_access);
    boost::upgrade_to_unique_lock<boost::shared_mutex> _uniqueLock(_lock);
    auto pr = dataToken.insert(p);
    dataAlive.insert(p);
    dataUserid.insert(p);
    return pr.second;
}

bool SessionManager::add(const Session &session, bool force) {
    auto p = std::make_shared<Session> (session);
    return add(p, force);
}

bool SessionManager::add(const xll &token, const xint &userid, const xll &alive, const xint &priority, bool force) {
    auto p = std::make_shared<Session> (token, userid, alive, priority);
    return add(p, force);
}

bool SessionManager::remove(const SessionManager::ptr &p) {
    boost::upgrade_lock<boost::shared_mutex> _lock(_access);
    boost::upgrade_to_unique_lock<boost::shared_mutex> _uniqueLock(_lock);
    try {
        dataAlive.erase(p);
        dataToken.erase(p);
        dataUserid.erase(p);
    } catch (std::exception &e) {
        cerr << "(remove)" << e.what() << '\n';
        return 0;
    }
    return 1;
}

bool SessionManager::removeByToken(const xll &token) {
    auto p = findToken(token);
    if (p == nullptr)
        return 0;
    return remove(p);
}

bool SessionManager::removeByUserid(const xint &userid) {
    auto p = findUserid(userid);
    if (p == nullptr)
        return 0;
    return remove(p);
}

bool SessionManager::setAliveTime(const xll &token, const xll &alive) {
    auto p = findToken(token);
    if (p == nullptr)
        return 0;
    if (!remove(p))
        return 0;
    p->setAlive(alive);
    if (alive < Session::getNowTime())
        return 0;
    return add(p);
}

SessionManager::ptr SessionManager::findToken(const xll &token) {
    auto p = std::make_shared<Session> (token);
    removeExpired();
    boost::shared_lock<boost::shared_mutex> _lock(_access);
    auto it = dataToken.find(p);
    if (it == dataToken.end())
        return nullptr;
    return *it;
}

SessionManager::ptr SessionManager::findUserid(const xint &userid) {
    auto p = std::make_shared<Session> (0, userid);
    removeExpired();
    boost::shared_lock<boost::shared_mutex> _lock(_access);
    auto it = dataUserid.find(p);
    if (it == dataUserid.end())
        return nullptr;
    return *it;
}

SessionManager::xll SessionManager::getRandToken() {
    auto getter = [this] {
        xll token = distr(eng);
        for (; token == 0; ++token);
        return token;
    };
    xll token = getter();
    for (; findToken(token) != nullptr; token = getter());
    return token;
}

SessionManager::xll SessionManager::getDefaultAlive() const {
    return defaultAlive;
}
