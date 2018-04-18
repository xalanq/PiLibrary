#ifndef BORROWRECORDMANAGER_H
#define BORROWRECORDMANAGER_H

#include "core.h"
#include "borrowrecord.h"
#include <set>
#include <vector>

class BorrowRecordManager {
public:
    void add(const BorrowRecord &record);
    void remove(const BorrowRecord &record);
    void clear();
    int size();
    std::vector<BorrowRecord> getData() const;

private:
    std::set<BorrowRecord> data;
};

#endif // BORROWRECORDMANAGER_H
