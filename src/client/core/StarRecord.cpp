// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/core/StarRecord.h>

StarRecord::~StarRecord() { }

StarRecord::xint StarRecord::getBookid() const {
    return bookid;
}

StarRecord& StarRecord::setBookid(const xint &value) {
    bookid = value;
    return *this;
}

StarRecord::xll StarRecord::getTime() const {
    return time;
}

StarRecord& StarRecord::setTime(const xll &value) {
    time = value;
    return *this;
}

StarRecord& StarRecord::setFromPtree(const ptree &pt) {
    setBookid(pt.get<xint>("bookid", 0));
    setTime(pt.get<xll>("time", 0));
    return *this;
}

StarRecord StarRecord::fromPtree(const ptree &pt) {
    StarRecord record;
    record.setFromPtree(pt);
    return record;
}

bool StarRecord::operator < (const StarRecord &b) const {
    return time == b.time ? bookid < b.bookid : time < b.time;
}