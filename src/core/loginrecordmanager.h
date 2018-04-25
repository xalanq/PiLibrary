#ifndef LOGINRECORDMANAGER_H
#define LOGINRECORDMANAGER_H

#include "xcore.h"
#include "loginrecord.h"
#include <set>
#include <vector>

class LoginRecordManager {
public:
    void add(const LoginRecord &record);
    void remove(const LoginRecord &record);
    void clear();
    int size();
    std::vector<LoginRecord> getData() const;

private:
    std::set<LoginRecord> data;
};

#endif // LOGINRECORDMANAGER_H
