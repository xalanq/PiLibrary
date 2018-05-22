// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/types.h>

namespace R {
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    extern size_t fileSize(const xstring &path);
    extern bool add(const xstring &path, const char *data, const size_t &size);
    extern char* get(const xstring &path, size_t &size);
}
