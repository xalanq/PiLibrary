// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/abstractuser.h>
#include <core/borrowrecord.h>
#include <core/browserecord.h>
#include <core/keeprecord.h>
#include <core/recordmanager.h>
#include <core/starrecord.h>
#include <core/xcore.h>

class User : public AbstractUser {
public:

    void addBorrowRecord(const BorrowRecord &record);
    RecordManager<BorrowRecord>& getBorrowRecordManager();
    void clearBorrowRecordManager();

    void addKeepRecord(const KeepRecord &record);
    RecordManager<KeepRecord>& getKeepRecordManager();
    void clearKeepRecordManager();

    void addBrowseRecord(const BrowseRecord &record);
    RecordManager<BrowseRecord>& getBrowseRecordManager();
    void clearBrowseRecordManager();

    void addStarRecord(const StarRecord &record);
    RecordManager<StarRecord>& getStarRecordManager();
    void clearStarRecordManager();

private:
    RecordManager<StarRecord> starRecordManager;
    RecordManager<BorrowRecord> borrowRecordManager;
    RecordManager<KeepRecord> keepRecordManager;
    RecordManager<BrowseRecord> browseRecordManager;
};
