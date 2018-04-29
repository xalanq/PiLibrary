#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include "xcore.h"
#include <ctime>

class BorrowRecord {
public:
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;

    uint getBookid() const;
    void setBookid(const uint &value);

    void setTime(const std::time_t &beginTime, const std::time_t &endTime);

    std::time_t getBeginTime() const;
    void setBeginTime(const std::time_t &value);

    std::time_t getEndTime() const;
    void setEndTime(const std::time_t &value);

private:
    uint bookid;
    std::time_t beginTime;
    std::time_t endTime;
};

#endif // BORROWRECORD_H
