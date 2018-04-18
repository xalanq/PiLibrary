#ifndef USER_H
#define USER_H

#include "core.h"
#include "abstractuser.h"
#include "loginrecordmanager.h"
#include "borrowrecordmanager.h"
#include "browserecordmanager.h"

class User : public AbstractUser {
public:

    BrowseRecordManager getBrowseRecord() const;
    void setBrowseRecord(const BrowseRecordManager &value);

    BorrowRecordManager getBorrowRecord() const;
    void setBorrowRecord(const BorrowRecordManager &value);

private:
    BrowseRecordManager browseRecord;
    BorrowRecordManager borrowRecord;
};

#endif // USER_H
