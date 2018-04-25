#ifndef RESOURCE_H
#define RESOURCE_H

#include "xcore.h"

namespace R {
    void remove(const X::string &path);
    void add(const X::string &path, const char *data);
}

#endif // RESOURCE_H
