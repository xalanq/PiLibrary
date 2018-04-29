#ifndef BOOK_H
#define BOOK_H

#include "xcore.h"
#include "bookcore.h"
#include <map>

class Book : public BookCore {
public:
    void addResource(const string &name, const string &path);
    void removeResource(const string &name);

private:
    std::map<string, string> resource;
};

#endif // BOOK_H
