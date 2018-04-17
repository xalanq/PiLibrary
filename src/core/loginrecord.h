#ifndef LOGINRECORD_H
#define LOGINRECORD_H

#include "xalanq.h"
#include <ctime>

class LoginRecord {
public:
    X::string getIp() const;
    void setIp(const X::string &value);

    std::time_t getTime() const;
    void setTime(const std::time_t &value);

    bool operator < (const LoginRecord &b) const;

private:
    X::string ip;
    std::time_t time;
};

#endif // LOGINRECORD_H
