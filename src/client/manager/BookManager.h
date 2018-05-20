// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <functional>
#include <map>
#include <queue>

#include <QObject>

#include <client/core/Book.h>
#include <client/core/BookBrief.h>
#include <client/manager/UserManager.h>
#include <core/types.h>

class BookManager : public QObject {
    Q_OBJECT

public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;

    BookManager(UserManager &userManager);

    bool hasBookBrief(const xint &bookid) const;

    bool hasBook(const xint &bookid) const;

    const BookBrief& getBookBrief(const xint &bookid) const;

    void getBookBrief(const xint &bookid, std::function<void(BookBrief &)> f, bool update = false);

    const Book& getBook(const xint &bookid) const;

    void getBook(const xint &bookid, std::function<void(Book &)> f, bool update = false);

public slots:

    void slotGetBookBrief(const X::ErrorCode &ec, const X::ptree &pt, std::function<void(BookBrief &)> f);

    void slotGetBook(const X::ErrorCode &ec, const X::ptree &pt, std::function<void(Book &)> f);

private:
    void _getBookBrief(const xint &bookid, std::function<void(BookBrief &)> f);

    void _getBook(const xint &bookid, std::function<void(Book &)> f);
    
    void popThread();

private:
    UserManager &userManager;
    std::map<xint, Book> mapBook;
    std::map<xint, BookBrief> mapBookBrief;
    std::queue<std::pair<xint, std::function<void(BookBrief &)>>> queueBookBrief {};
    std::queue<std::pair<xint, std::function<void(Book &)>>> queueBook {};
    int threadCounts {};
    int threadLimit {};
};