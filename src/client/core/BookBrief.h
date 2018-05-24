// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/Resource.h>
#include <core/types.h>

class BookBrief {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;
    
    virtual ~BookBrief() = default;

    xint getBookid() const;
    BookBrief& setBookid(const xint &value);

    xstring getTitle() const;
    BookBrief& setTitle(const xstring &value);

    xstring getAuthor() const;
    BookBrief& setAuthor(const xstring &value);

    xstring getISBN() const;
    BookBrief& setISBN(const xstring &value);

    xstring getPublisher() const;
    BookBrief& setPublisher(const xstring &value);

    xstring getIntroduction() const;
    BookBrief& setIntroduction(const xstring &value);

    xint getPriority() const;
    BookBrief& setPriority(const xint &value);

    xint getStarCount() const;
    BookBrief& setStarCount(const xint &value);

    xll getMaxKeepTime() const;
    BookBrief& setMaxKeepTime(const xll &value);

    Resource getCover() const;
    BookBrief& setCover(const Resource &cover);
    
    virtual BookBrief& setFromPtree(const ptree &pt);
    static BookBrief fromPtree(const ptree &pt); 

    BookBrief& updateFromPtree(const ptree &pt);

    static const BookBrief& unknown();

private:
    xint bookid {};
    xstring title {};
    xstring author {};
    xstring ISBN {};
    xstring publisher {};
    xstring introduction {};
    xint priority {};
    xll maxKeepTime {};
    xint starCount {};
    Resource cover {};
};
