// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/xcore.h>

namespace R {
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    void remove(const xstring &path);
    void add(const xstring &path, const char *data);
}
