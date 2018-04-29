#include "loginrecordmanager.h"

void LoginRecordManager::add(const LoginRecord &record) {
    data.insert(record);
}

void LoginRecordManager::remove(const LoginRecord &record) {
    data.erase(data.find(record));
}

void LoginRecordManager::clear() {
    data.clear();
}

int LoginRecordManager::size() {
    return data.size();
}

std::vector<LoginRecord> LoginRecordManager::getData() const {
    std::vector<LoginRecord> ret;
    for (const auto &i : data)
        ret.push_back(i);
    return ret;
}
