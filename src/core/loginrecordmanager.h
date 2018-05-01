// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <set>
#include <vector>

#include <core/loginrecord.h>
#include <core/xcore.h>

class LoginRecordManager {
public:
    void add(const LoginRecord &record);
    void remove(const LoginRecord &record);
    void clear();
    size_t size();
    std::vector<LoginRecord> getData() const;

private:
    std::set<LoginRecord> data;
};
