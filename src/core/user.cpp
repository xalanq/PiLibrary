#include "user.h"

BrowseRecordManager User::getBrowseRecord() const {
    return browseRecord;
}

void User::setBrowseRecord(const BrowseRecordManager &value) {
    browseRecord = value;
}

BorrowRecordManager User::getBorrowRecord() const {
    return borrowRecord;
}

void User::setBorrowRecord(const BorrowRecordManager &value) {
    borrowRecord = value;
}
