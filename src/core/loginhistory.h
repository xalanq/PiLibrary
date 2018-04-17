#ifndef LOGINHISTORY_H
#define LOGINHISTORY_H

#include <ctime>
#include <string>

class LoginHistory {
public:
    LoginHistory();

private:
    std::string ip;
    std::time_t time;
};

#endif // LOGINHISTORY_H
