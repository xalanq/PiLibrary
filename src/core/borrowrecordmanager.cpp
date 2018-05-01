// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/borrowrecordmanager.h>

void BorrowRecordManager::add(const BorrowRecord &record) {
    data.insert(record);
}

void BorrowRecordManager::remove(const BorrowRecord &record) {
    data.erase(data.find(record));
}

void BorrowRecordManager::clear() {
    data.clear();
}

size_t BorrowRecordManager::size() {
    return data.size();
}

std::vector<BorrowRecord> BorrowRecordManager::getData() const {
    std::vector<BorrowRecord> ret;
    for (const auto &i : data)
        ret.push_back(i);
    return ret;
}
