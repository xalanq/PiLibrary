#ifndef BROWSERECORD_H
#define BROWSERECORD_H

#include "xalanq.h"
#include <ctime>

class BrowseRecord {
public:
    X::uint getBookid() const;
    void setBookid(const X::uint &value);

    std::time_t getTime() const;
    void setTime(const std::time_t &value);

private:
    X::uint bookid;
    std::time_t time;
};

#endif // BROWSERECORD_H
