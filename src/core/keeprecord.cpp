// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/keeprecord.h>

KeepRecord::~KeepRecord() { }

KeepRecord::xint KeepRecord::getBookid() const {
    return bookid;
}

void KeepRecord::setBookid(const xint &value) {
    bookid = value;
}

void KeepRecord::setTime(const xll &beginTime, const xll &endTime) {
    setBeginTime(beginTime);
    setEndTime(endTime);
}

KeepRecord::xll KeepRecord::getBeginTime() const {
    return beginTime;
}

void KeepRecord::setBeginTime(const xll &value) {
    beginTime = value;
}

KeepRecord::xll KeepRecord::getEndTime() const {
    return endTime;
}

void KeepRecord::setEndTime(const xll &value) {
    endTime = value;
}

KeepRecord KeepRecord::fromPtree(const ptree &pt) {
    KeepRecord record;
    record.setBookid(pt.get<xint>("bookid", 0));
    record.setTime(pt.get<xll>("beginTime", 0), pt.get<xll>("endTime", 0));
    return std::move(record);
}

bool KeepRecord::operator < (const KeepRecord &t) const {
    return endTime == t.endTime ? (beginTime == t.beginTime ? bookid < t.bookid : beginTime < t.beginTime) : endTime < t.endTime;
}
