// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <map>

#include <core/bookcore.h>
#include <core/resource.h>
#include <core/xcore.h>

class Book : public BookCore {
public:
    void addResource(const string &name, const string &path);
    void removeResource(const string &name);

private:
    std::map<string, string> resource;
};
