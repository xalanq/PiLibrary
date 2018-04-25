#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "xserver.h"
#include "session.h"
#include <set>
#include <ctime>
#include <memory>

class SessionManager {
public:
    typedef std::shared_ptr<Session> ptr;

    SessionManager();
    void removeExpired();
    bool add(const ptr &p);
    bool add(const Session &session);
    bool add(const X::uint &token, const X::uint &userid, const std::time_t &alive);
    void remove(const ptr &p);
    bool removeByToken(const X::uint &token);
    bool removeByUserid(const X::uint &userid);
    bool setAliveTime(const X::uint &token, const std::time_t &alive);
    ptr findToken(const X::uint &token);
    ptr findUserid(const X::uint &userid);
    X::uint getRandToken();

private:
    std::multiset<ptr, [](const ptr &a, const ptr &b) { return a->getAlive() < b->getAlive(); }> dataAlive;
    std::set<ptr, [](const ptr &a, const ptr &b) { return a->getToken() < b->getToken(); }> dataToken;
    std::set<ptr, [](const ptr &a, const ptr &b) { return a->getUserid() < b->getUserid(); }> dataUserid;
};

#endif // SESSIONMANAGER_H
