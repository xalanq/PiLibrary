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
    
    Book& setFromPtree(const ptree &pt);
    static Book fromPtree(const ptree &pt); 

    Book& updateFromPtree(const ptree &pt);

    Book& cleanCover();

    static const Book& unknown();

private:
    xint amount {};
    xstring position {};
};
