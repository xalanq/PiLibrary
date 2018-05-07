// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <set>
#include <vector>

#include <core/xcore.h>

template<typename T>
class RecordManager {
public:
    void add(const T &record);
    void remove(const T &record);
    void clear();
    size_t size();
    std::vector<T> getData() const;

private:
    std::set<T> data;
};

template<typename T>
void RecordManager<T>::add(const T &record) {
    data.insert(record);
}

template<typename T>
void RecordManager<T>::remove(const T &record) {
    data.erase(data.find(record));
}

template<typename T>
void RecordManager<T>::clear() {
    data.clear();
}

template<typename T>
size_t RecordManager<T>::size() {
    return data.size();
}

template<typename T>
std::vector<T> RecordManager<T>::getData() const {
    std::vector<T> ret;
    for (const auto &i : data)
        ret.push_back(i);
    return ret;
}
