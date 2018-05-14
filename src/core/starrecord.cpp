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

bool StarRecord::operator < (const StarRecord &b) const {
    return time == b.time ? bookid < b.bookid : time < b.time;
}