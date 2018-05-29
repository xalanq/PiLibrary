// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadGetRecord.h>
#include <client/object/GetRecords.h>

// I hate Qt that it can't template with Q_OBJECT

#define WTF(CLASS_NAME, TYPE_NAME, ACTION_CODE, ARRAY_NAME) \
CLASS_NAME::CLASS_NAME(const X::xll &token, BookManager &bookManager, const X::xint &number, const X::xint &begin, QObject *parent) : \
    QObject(parent), \
    token(token), \
    bookManager(bookManager), \
    number(number), \
    begin(begin) { \
} \
void CLASS_NAME::start() { \
    auto thread = new ThreadGetRecord(token, ACTION_CODE, number, begin, this); \
    connect(thread, &ThreadGetRecord::done, this, &CLASS_NAME::slotEnd); \
    connect(thread, &ThreadGetRecord::finished, thread, &QObject::deleteLater); \
    thread->start(); \
} \
void CLASS_NAME::slotEnd(const X::ErrorCode &ec, const X::ptree &pt) { \
    if (ec != X::NoError) \
        return; \
    auto arr = pt.get_child(ARRAY_NAME); \
    recordList.clear(); \
    for (auto &&child : arr) { \
        auto record = TYPE_NAME::fromPtree(child.second); \
        recordList.push_back(record); \
    } \
    emit done(recordList); \
    deleteLater(); \
}

WTF(GetBrowseRecords, BrowseRecord, X::GetBrowseRecord, "browseRecord")
WTF(GetKeepRecords, KeepRecord, X::GetKeepRecord, "keepRecord")
WTF(GetBorrowRecords, BorrowRecord, X::GetBorrowRecord, "borrowRecord")
WTF(GetStarRecords, StarRecord, X::GetStarRecord, "starRecord")