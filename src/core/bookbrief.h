// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/xcore.h>

class BookBrief {
public:
    virtual ~BookBrief() = 0;
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    xint getBookid() const;
    void setBookid(const xint &value);

    xstring getTitle() const;
    void setTitle(const xstring &value);

    xstring getAuthor() const;
    void setAuthor(const xstring &value);

    xstring getISBN() const;
    void setISBN(const xstring &value);

    xstring getPublisher() const;
    void setPublisher(const xstring &value);

    xstring getIntroduction() const;
    void setIntroduction(const xstring &value);

    xint getPriority() const;
    void setPriority(const xint &value);

private:
    xint bookid {0};
    xstring title {""};
    xstring author {""};
    xstring ISBN {""};
    xstring publisher {""};
    xstring introduction {""};
    xint priority {0};
};
