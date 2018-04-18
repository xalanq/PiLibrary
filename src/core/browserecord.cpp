#include "browserecord.h"

X::uint BrowseRecord::getBookid() const {
    return bookid;
}

void BrowseRecord::setBookid(const X::uint &value) {
    bookid = value;
}

std::time_t BrowseRecord::getTime() const {
    return time;
}

void BrowseRecord::setTime(const std::time_t &value) {
    time = value;
}

bool BrowseRecord::operator < (const BrowseRecord &a) const {
    return time == a.time ? bookid < a.bookid : time < a.time;
}
