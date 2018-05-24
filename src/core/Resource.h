// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/types.h>

class Resource {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    Resource(char *data = nullptr, const size_t &size = 0);
    Resource(const Resource &r);
    Resource(Resource &&r);
    Resource& operator = (const Resource &r);
    Resource& operator = (Resource &&r);
    
    char* getData();
    const char* getData() const;
    Resource& setData(char *data);
    
    size_t getSize() const;
    Resource& setSize(const size_t &size);
    
    void clean();

    static size_t fileSize(const xstring &path);
    static bool add(const xstring &path, const Resource &file);
    static Resource get(const xstring &path);
    static Resource copy(const Resource &file);
    
private:
    char *data {};
    size_t size {};
};
