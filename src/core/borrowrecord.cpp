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

bool BorrowRecord::operator < (const BorrowRecord &t) const {
    return beginTime == t.beginTime ? (endTime == t.endTime ? bookid < t.bookid : endTime < t.endTime) : beginTime < t.beginTime;
}
