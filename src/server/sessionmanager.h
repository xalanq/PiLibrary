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

    struct compareAlive {
        bool operator () (const ptr &a, const ptr &b) const {
            return a->getAlive() < b->getAlive();
        }
    };
    struct compareToken {
        bool operator () (const ptr &a, const ptr &b) const {
            return a->getToken() < b->getToken();
        }
    };
    struct compareUserid {
        bool operator () (const ptr &a, const ptr &b) const {
            return a->getUserid() < b->getUserid();
        }
    };

private:
    std::multiset<ptr, compareAlive> dataAlive;
    std::set<ptr, compareToken> dataToken;
    std::set<ptr, compareUserid> dataUserid;
};

#endif // SESSIONMANAGER_H
