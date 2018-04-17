#ifndef LOGINHISTORYMANAGER_H
#define LOGINHISTORYMANAGER_H

#include <vector>
#include "loginhistory.h"

class LoginHistoryManager {
public:
    LoginHistoryManager();

private:
    std::vector<LoginHistory> data;
};

#endif // LOGINHISTORYMANAGER_H
