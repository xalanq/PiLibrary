#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include "core.h"
#include <ctime>

class BorrowRecord {
public:
    X::uint getBookid() const;
    void setBookid(const X::uint &value);

    void setTime(const std::time_t &beginTime, const std::time_t &endTime);

    std::time_t getBeginTime() const;
    void setBeginTime(const std::time_t &value);

    std::time_t getEndTime() const;
    void setEndTime(const std::time_t &value);

private:
    X::uint bookid;
    std::time_t beginTime;
    std::time_t endTime;
};

#endif // BORROWRECORD_H
