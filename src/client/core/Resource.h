// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

class Resource {
public:
    char* getData();
    const char* getData() const;
    Resource& setData(char *data);
    
    size_t getSize() const;
    Resource& setSize(const size_t &size);
    
    void clean();
    
private:
    char *data {};
    size_t size {};
};
