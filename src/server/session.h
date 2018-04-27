#ifndef SESSION_H
#define SESSION_H

#include "xserver.h"
#include <ctime>
#include <cstdlib>

class Session {
public:
    Session(const X::ull &token = 0, const X::uint &userid = 0, const std::time_t &alive = 0);

    static time_t getNowTime();

    X::ull getToken() const;
    void setToken(const X::ull &value);

    X::uint getUserid() const;
    void setUserid(const X::uint &value);

    std::time_t getAlive() const;
    void setAlive(const std::time_t &value);

private:
    X::ull token;
    X::uint userid;
    std::time_t alive;
};

#endif // SESSION_H
