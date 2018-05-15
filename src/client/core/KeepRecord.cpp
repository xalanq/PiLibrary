// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/core/KeepRecord.h>

KeepRecord::~KeepRecord() { }

KeepRecord::xint KeepRecord::getBookid() const {
    return bookid;
}

KeepRecord& KeepRecord::setBookid(const xint &value) {
    bookid = value;
    return *this;
}

KeepRecord::xll KeepRecord::getBeginTime() const {
    return beginTime;
}

KeepRecord& KeepRecord::setBeginTime(const xll &value) {
    beginTime = value;
    return *this;
}

KeepRecord::xll KeepRecord::getEndTime() const {
    return endTime;
}

KeepRecord& KeepRecord::setEndTime(const xll &value) {
    endTime = value;
    return *this;
}

KeepRecord& KeepRecord::setTime(const xll &beginTime, const xll &endTime) {
    setBeginTime(beginTime);
    setEndTime(endTime);
    return *this;
}

KeepRecord& KeepRecord::setFromPtree(const ptree &pt) {
    setBookid(pt.get<xint>("bookid", 0));
    setTime(pt.get<xll>("beginTime", 0), pt.get<xll>("endTime", 0));
    return *this;
}

KeepRecord KeepRecord::fromPtree(const ptree &pt) {
    KeepRecord record;
    record.setFromPtree(pt);
    return std::move(record);
}

bool KeepRecord::operator < (const KeepRecord &t) const {
    return endTime == t.endTime ? (beginTime == t.beginTime ? bookid < t.bookid : beginTime < t.beginTime) : endTime < t.endTime;
}
