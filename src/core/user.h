// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/abstractuser.h>
#include <core/borrowrecordmanager.h>
#include <core/browserecordmanager.h>
#include <core/loginrecordmanager.h>
#include <core/xcore.h>

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
