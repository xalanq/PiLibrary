// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/user.h>

void User::addBorrowRecord(const BorrowRecord &record) {
    borrowRecordManager.add(record);
}

RecordManager<BorrowRecord>& User::getBorrowRecordManager() {
    return borrowRecordManager;
}

void User::clearBorrowRecordManager() {
    borrowRecordManager.clear();
}

void User::addKeepRecord(const BorrowRecord &record) {
    keepRecordManager.add(record);
}

RecordManager<BorrowRecord>& User::getKeepRecordManager() {
    return keepRecordManager;
}

void User::clearKeepRecordManager() {
    keepRecordManager.clear();
}

void User::addBrowseRecord(const BrowseRecord &record) {
    browseRecordManager.add(record);
}

RecordManager<BrowseRecord>& User::getBrowseRecordManager() {
    return browseRecordManager;
}

void User::clearBrowseRecordManager() {
    browseRecordManager.clear();
}
