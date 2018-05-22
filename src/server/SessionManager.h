// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <memory>
#include <random>
#include <set>

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <core/types.h>
#include <server/Session.h>

class SessionManager {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;
    typedef std::shared_ptr<Session> ptr;

    SessionManager(const xll &defaultAlive);
    void removeExpired();
    
    bool add(const ptr &p, bool force = false);
    bool add(const Session &session, bool force = false);
    bool add(const xll &token, const xint &userid, const xll &alive, const xint &priority, bool force = false);
    
    bool remove(const ptr &p);
    bool removeByToken(const xll &token);
    bool removeByUserid(const xint &userid);
    
    bool setAliveTime(const xll &token, const xll &alive);
    
    ptr findToken(const xll &token);
    ptr findUserid(const xint &userid);
    
    xll getRandToken();
    
    xll getDefaultAlive() const;

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
    std::uniform_int_distribution<xll> distr;
    std::mt19937_64 eng;
    xll defaultAlive;
    boost::shared_mutex _access;
};
