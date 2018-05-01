// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <set>
#include <vector>

#include <core/borrowrecord.h>
#include <core/xcore.h>

class BorrowRecordManager {
public:
    void add(const BorrowRecord &record);
    void remove(const BorrowRecord &record);
    void clear();
    size_t size();
    std::vector<BorrowRecord> getData() const;

private:
    std::set<BorrowRecord> data;
};
