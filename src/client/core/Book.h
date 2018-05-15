// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <map>

#include <client/core/BookBrief.h>

class Book : public BookBrief {
public:
    xint getAmount() const;
    Book& setAmount(const xint &value);

    xstring getPosition() const;
    Book& setPosition(const xstring &value);

    xint getStarCount() const;
    Book& setStarCount(const xint &value);
    
    Book& addResource(const xstring &name, const xstring &path);
    Book& removeResource(const xstring &name);
    
    Book& setFromPtree(const ptree &pt);
    static Book fromPtree(const ptree &pt); 

private:
    xint amount{};
    xstring position{};
    xint starCount{};
    std::map<xstring, xstring> resource{};
};
