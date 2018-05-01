// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <set>
#include <vector>

#include <core/browserecord.h>
#include <core/xcore.h>

class BrowseRecordManager {
public:
    void add(const BrowseRecord &record);
    void remove(const BrowseRecord &record);
    void clear();
    size_t size();
    std::vector<BrowseRecord> getData() const;

private:
    std::set<BrowseRecord> data;
};
