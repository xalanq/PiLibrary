// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/starrecord.h>

StarRecord::~StarRecord() { }

StarRecord::xint StarRecord::getBookid() const {
    return bookid;
}

void StarRecord::setBookid(const xint &value) {
    bookid = value;
}

StarRecord::xll StarRecord::getTime() const {
    return time;
}

void StarRecord::setTime(const xll &value) {
    time = value;
}

StarRecord StarRecord::fromPtree(const ptree &pt) {
    StarRecord record;
    record.setBookid(pt.get<xint>("bookid", 0));
    record.setTime(pt.get<xll>("time"));
    return record;
}

bool StarRecord::operator < (const StarRecord &b) const {
    return time == b.time ? bookid < b.bookid : time < b.time;
}