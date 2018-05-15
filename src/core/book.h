// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <map>

#include <core/bookbrief.h>
#include <core/xcore.h>
#include <core/resource.h>

class Book : public BookBrief {
public:
    void addResource(const xstring &name, const xstring &path);
    void removeResource(const xstring &name);

    xint getAmount() const;
    void setAmount(const xint &value);

    xstring getPosition() const;
    void setPosition(const xstring &value);

    xint getStarCount() const;
    void setStarCount(const xint &value);

private:
    xint amount {0};
    xstring position {""};
    xint starCount {0};
    std::map<xstring, xstring> resource;
};
