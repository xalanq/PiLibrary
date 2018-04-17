#ifndef BOOK_H
#define BOOK_H

#include "xalanq.h"
#include "bookcore.h"
#include <map>

class Book : public BookCore {
public:
    void addResource(const X::string &name, const X::string &path);
    void removeResource(const X::string &name);

private:
    std::map<X::string, X::string> resource;
};

#endif // BOOK_H
