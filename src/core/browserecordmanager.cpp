// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/browserecordmanager.h>

void BrowseRecordManager::add(const BrowseRecord &record) {
    data.insert(record);
}

void BrowseRecordManager::remove(const BrowseRecord &record) {
    data.erase(data.find(record));
}

void BrowseRecordManager::clear() {
    data.clear();
}

size_t BrowseRecordManager::size() {
    return data.size();
}

std::vector<BrowseRecord> BrowseRecordManager::getData() const {
    std::vector<BrowseRecord> ret;
    for (const auto &i : data)
        ret.push_back(i);
    return ret;
}
