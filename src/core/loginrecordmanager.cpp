// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/loginrecordmanager.h>

void LoginRecordManager::add(const LoginRecord &record) {
    data.insert(record);
}

void LoginRecordManager::remove(const LoginRecord &record) {
    data.erase(data.find(record));
}

void LoginRecordManager::clear() {
    data.clear();
}

size_t LoginRecordManager::size() {
    return data.size();
}

std::vector<LoginRecord> LoginRecordManager::getData() const {
    std::vector<LoginRecord> ret;
    for (const auto &i : data)
        ret.push_back(i);
    return ret;
}
