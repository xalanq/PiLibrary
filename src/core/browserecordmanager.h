#ifndef BROWSERECORDMANAGER_H
#define BROWSERECORDMANAGER_H

#include "core.h"
#include "browserecord.h"
#include <set>
#include <vector>

class BrowseRecordManager {
public:
    void add(const BrowseRecord &record);
    void remove(const BrowseRecord &record);
    void clear();
    int size();
    std::vector<BrowseRecord> getData() const;

private:
    std::set<BrowseRecord> data;
};

#endif // BROWSERECORDMANAGER_H
