#ifndef BROWSERECORD_H
#define BROWSERECORD_H

#include "xcore.h"
#include <ctime>

class BrowseRecord {
public:
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;
    
    uint getBookid() const;
    void setBookid(const uint &value);

    std::time_t getTime() const;
    void setTime(const std::time_t &value);

    bool operator < (const BrowseRecord &a) const;

private:
    uint bookid;
    std::time_t time;
};

#endif // BROWSERECORD_H
