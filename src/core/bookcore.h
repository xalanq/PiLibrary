// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/xcore.h>

class BookCore {
public:
    virtual ~BookCore() = 0;
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

    xint getAmount() const;
    void setAmount(const xint &value);

    xstring getIntroduction() const;
    void setIntroduction(const xstring &value);

    xstring getPosition() const;
    void setPosition(const xstring &value);

    xint getPriority() const;
    void setPriority(const xint &value);

    xint getStarCount() const;
    void setStarCount(const xint &value);

private:
    xint bookid;
    xstring title;
    xstring author;
    xstring ISBN;
    xstring publisher;
    xint amount;
    xstring introduction;
    xstring position;
    xint priority;
    xint starCount;
};
