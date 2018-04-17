#include "book.h"

void Book::addResource(const X::string &name, const X::string &path) {
    resource[name] = path;
}

void Book::removeResource(const X::string &name) {
    auto it = resource.find(name);
    if (it != resource.end()) {
        R::remove(*it);
    }
}
