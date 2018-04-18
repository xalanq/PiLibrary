#include "browserecordmanager.h"

void BrowseRecordManager::add(const BrowseRecord &record) {
    data.insert(record);
}

void BrowseRecordManager::remove(const BrowseRecord &record) {
    data.erase(data.find(record));
}

void BrowseRecordManager::clear() {
    data.clear();
}

int BrowseRecordManager::size() {
    return data.size();
}

std::vector<BrowseRecord> BrowseRecordManager::getData() const {
    std::vector<BrowseRecord> ret;
    for (auto i : data)
        ret.push_back(i);
    return ret;
}
