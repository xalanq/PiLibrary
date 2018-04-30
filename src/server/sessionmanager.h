#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "xserver.h"
#include "session.h"
#include <set>
#include <ctime>
#include <memory>
#include <random>

class SessionManager {
public:
    typedef std::shared_ptr<Session> ptr;
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;

    SessionManager(const uint &defaulAlive);
    void removeExpired();
    bool add(const ptr &p);
    bool add(const Session &session);
    bool add(const ull &token, const uint &userid, const std::time_t &alive, const uint &priority);
    void remove(const ptr &p);
    bool removeByToken(const ull &token);
    bool removeByUserid(const uint &userid);
    bool setAliveTime(const ull &token, const std::time_t &alive);
    ptr findToken(const ull &token);
    ptr findUserid(const uint &userid);
    ull getRandToken();
    uint getDefaultAlive() const;

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
    std::uniform_int_distribution<ull> distr;
    std::mt19937_64 eng;
    uint defaultAlive;
};

#endif // SESSIONMANAGER_H
