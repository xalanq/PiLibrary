#ifndef SESSION_H
#define SESSION_H

#include "xserver.h"
#include <ctime>
#include <cstdlib>

class Session {
public:
    Session(const X::uint &token = 0, const X::uint &userid = 0, const std::time_t &alive = 0);

    static X::uint getRandToken();
    static time_t getNowTime();

    X::uint getToken() const;
    void setToken(const X::uint &value);

    X::uint getUserid() const;
    void setUserid(const X::uint &value);

    std::time_t getAlive() const;
    void setAlive(const std::time_t &value);

private:
    X::uint token;
    X::uint userid;
    std::time_t alive;
};

#endif // SESSION_H
