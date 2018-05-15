// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/core/BrowseRecord.h>

BrowseRecord::xint BrowseRecord::getBookid() const {
    return bookid;
}

BrowseRecord& BrowseRecord::setBookid(const xint &value) {
    bookid = value;
    return *this;
}

BrowseRecord::xll BrowseRecord::getTime() const {
    return time;
}

BrowseRecord& BrowseRecord::setTime(const xll &value) {
    time = value;
    return *this;
}

BrowseRecord& BrowseRecord::setFromPtree(const ptree &pt) {
    setBookid(pt.get<xint>("bookid", 0));
    setTime(pt.get<xint>("time", 0));
    return *this;
}

BrowseRecord BrowseRecord::fromPtree(const ptree &pt) {
    BrowseRecord record;
    record.setFromPtree(pt);
    return std::move(record);
}

bool BrowseRecord::operator < (const BrowseRecord &a) const {
    return time == a.time ? bookid < a.bookid : time < a.time;
}
