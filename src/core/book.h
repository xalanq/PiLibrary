// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <map>

#include <core/bookcore.h>
#include <core/resource.h>
#include <core/xcore.h>

class Book : public BookCore {
public:
    void addResource(const xstring &name, const xstring &path);
    void removeResource(const xstring &name);

private:
    std::map<xstring, xstring> resource;
};
