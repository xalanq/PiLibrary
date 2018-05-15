// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/core/BorrowRecord.h>

BorrowRecord::~BorrowRecord() { }

BorrowRecord::xint BorrowRecord::getBookid() const {
    return bookid;
}

BorrowRecord& BorrowRecord::setBookid(const xint &value) {
    bookid = value;
    return *this;
}

BorrowRecord::xll BorrowRecord::getBeginTime() const {
    return beginTime;
}

BorrowRecord& BorrowRecord::setBeginTime(const xll &value) {
    beginTime = value;
    return *this;
}

BorrowRecord::xll BorrowRecord::getEndTime() const {
    return endTime;
}

BorrowRecord& BorrowRecord::setEndTime(const xll &value) {
    endTime = value;
    return *this;
}

BorrowRecord::xll BorrowRecord::getReturnTime() const {
    return returnTime;
}

BorrowRecord& BorrowRecord::setReturnTime(const xll &value) {
    returnTime = value;
    return *this;
}

BorrowRecord& BorrowRecord::setTime(const xll &beginTime, const xll &endTime, const xll &returnTime) {
    setBeginTime(beginTime);
    setEndTime(endTime);
    setReturnTime(returnTime);
    return *this;
}

BorrowRecord& BorrowRecord::setFromPtree(const ptree &pt) {
    setBookid(pt.get<xint>("bookid", 0));
    setTime(pt.get<xll>("beginTime", 0), pt.get<xll>("endTime", 0), pt.get<xll>("returnTime", 0));
    return *this;
}

BorrowRecord BorrowRecord::fromPtree(const ptree &pt) {
    BorrowRecord record;
    record.setFromPtree(pt);
    return std::move(record);
}

bool BorrowRecord::operator < (const BorrowRecord &t) const {
    return returnTime == t.returnTime ? (endTime == t.endTime ? (beginTime == t.beginTime ? bookid < t.bookid : beginTime < t.beginTime) : endTime < t.endTime) : returnTime < t.returnTime;
}
