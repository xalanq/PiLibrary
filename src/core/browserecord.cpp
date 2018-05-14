// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/browserecord.h>

BrowseRecord::xint BrowseRecord::getBookid() const {
    return bookid;
}

void BrowseRecord::setBookid(const xint &value) {
    bookid = value;
}

BrowseRecord::xll BrowseRecord::getTime() const {
    return time;
}

void BrowseRecord::setTime(const xll &value) {
    time = value;
}

BrowseRecord BrowseRecord::fromPtree(const ptree &pt) {
    BrowseRecord record;
    record.setBookid(pt.get<xint>("bookid", 0));
    record.setTime(pt.get<xint>("time", 0));
    return std::move(record);
}

bool BrowseRecord::operator < (const BrowseRecord &a) const {
    return time == a.time ? bookid < a.bookid : time < a.time;
}
