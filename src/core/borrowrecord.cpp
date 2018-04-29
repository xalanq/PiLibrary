#include "borrowrecord.h"

BorrowRecord::uint BorrowRecord::getBookid() const {
    return bookid;
}

void BorrowRecord::setBookid(const uint &value) {
    bookid = value;
}

void BorrowRecord::setTime(const time_t &beginTime, const time_t &endTime) {
    this->beginTime = beginTime;
    this->endTime = endTime;
}

std::time_t BorrowRecord::getBeginTime() const {
    return beginTime;
}

void BorrowRecord::setBeginTime(const std::time_t &value) {
    beginTime = value;
}

std::time_t BorrowRecord::getEndTime() const {
    return endTime;
}

void BorrowRecord::setEndTime(const std::time_t &value) {
    endTime = value;
}
