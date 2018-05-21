// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadGetRecord.h>
#include <client/thread/ThreadGetBookBrief.h>
#include <client/utils/GetRecords.h>

// I hate Qt that it can't template with Q_OBJECT

#define WTF(CLASS_NAME, TYPE_NAME, ACTION_CODE, ARRAY_NAME) \
Get##CLASS_NAME::Get##CLASS_NAME(const X::xll &token, BookManager &bookManager, const X::xint &number, const X::xint &begin, QObject *parent) : \
    token(token), \
    bookManager(bookManager), \
    number(number), \
    begin(begin), \
    QObject(parent) { \
} \
void Get##CLASS_NAME::start() { \
    auto thread = new ThreadGetRecord(token, ##ACTION_CODE, number, begin, this); \
    connect(thread, &ThreadGetRecord::done, this, &Get##CLASS_NAME::slotEnd); \
    connect(thread, &ThreadGetRecord::finished, thread, &QObject::deleteLater); \
    thread->start(); \
} \
void Get##CLASS_NAME::slotEnd(const X::ErrorCode &ec, const X::ptree &pt) { \
    if (ec != X::NoError) \
        return; \
    auto arr = pt.get_child(ARRAY_NAME); \
    recordList.clear(); \
    for (auto &&child : arr) { \
        auto record = ##TYPE_NAME::fromPtree(child.second); \
        recordList.push_back(record); \
    } \
    emit done(recordList); \
    deleteLater(); \
}

WTF(BrowseRecords, BrowseRecord, X::GetBrowseRecord, "browseRecord")
WTF(KeepRecords, KeepRecord, X::GetKeepRecord, "keepRecord")
WTF(BorrowRecords, BorrowRecord, X::GetBorrowRecord, "borrowRecord")
WTF(StarRecords, StarRecord, X::GetStarRecord, "starRecord")