// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/borrowrecord.h>

BorrowRecord::~BorrowRecord() { }

BorrowRecord::xint BorrowRecord::getBookid() const {
    return bookid;
}

void BorrowRecord::setBookid(const xint &value) {
    bookid = value;
}

void BorrowRecord::setTime(const xll &beginTime, const xll &endTime, const xll &returnTime) {
    setBeginTime(beginTime);
    setEndTime(endTime);
    setReturnTime(returnTime);
}

BorrowRecord::xll BorrowRecord::getBeginTime() const {
    return beginTime;
}

void BorrowRecord::setBeginTime(const xll &value) {
    beginTime = value;
}

BorrowRecord::xll BorrowRecord::getEndTime() const {
    return endTime;
}

void BorrowRecord::setEndTime(const xll &value) {
    endTime = value;
}

BorrowRecord::xll BorrowRecord::getReturnTime() const {
    return returnTime;
}

void BorrowRecord::setReturnTime(const xll &value) {
    returnTime = value;
}

BorrowRecord BorrowRecord::fromPtree(const ptree &pt) {
    BorrowRecord record;
    record.setBookid(pt.get<xint>("bookid", 0));
    record.setTime(pt.get<xll>("beginTime", 0), pt.get<xll>("endTime", 0), pt.get<xll>("returnTime", 0));
    return std::move(record);
}

bool BorrowRecord::operator < (const BorrowRecord &t) const {
    return returnTime == t.returnTime ? (endTime == t.endTime ? (beginTime == t.beginTime ? bookid < t.bookid : beginTime < t.beginTime) : endTime < t.endTime) : returnTime < t.returnTime;
}
