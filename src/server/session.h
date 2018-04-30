#ifndef SESSION_H
#define SESSION_H

#include "xserver.h"
#include <ctime>
#include <cstdlib>

class Session {
public:
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;

    Session(const ull &token = 0, const uint &userid = 0, const std::time_t &alive = 0, const uint &priority = 0);

    static time_t getNowTime();

    ull getToken() const;
    void setToken(const ull &value);

    uint getUserid() const;
    void setUserid(const uint &value);

    std::time_t getAlive() const;
    void setAlive(const std::time_t &value);

    uint getPriority() const;
    void setPriority(const uint &value);

private:
    ull token;
    uint userid;
    std::time_t alive;
    uint priority;
};

#endif // SESSION_H
