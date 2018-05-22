// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <functional>
#include <map>
#include <vector>
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
    typedef X::ErrorCode ErrorCode;
    typedef X::ActionCode ActionCode;

    BookManager(UserManager &userManager);

    bool hasBook(const xint &bookid) const;
    bool hasBookBrief(const xint &bookid) const;

    const Book& getBook(const xint &bookid) const;
    void getBook(const xint &bookid, std::function<void(Book &)> f, bool update = false);

    const BookBrief& getBookBrief(const xint &bookid) const;
    void getBookBrief(const xint &bookid, std::function<void(BookBrief &)> f, bool update = false);

    void updateStar(const xint &bookid, bool star);

    void installBrowseEvent(std::function<void(xint &)> f);

public slots:
    void slotGetBook(const ErrorCode &ec, const ptree &pt, const xint &bookid, const Resource &cover);
    void slotGetBookBrief(const ErrorCode &ec, const ptree &pt, const xint &bookid, const Resource &cover);

private:
    void _getBook(const xint &bookid);
    void _getBookBrief(const xint &bookid);

    void popThread();
    
    void emitBrowseEvents(const xint &bookid);

private:
    UserManager &userManager;
    std::map<xint, Book> mapBook;
    std::map<xint, BookBrief> mapBookBrief;
    std::queue<xint> queueBook;
    std::queue<xint> queueBookBrief;
    std::map<xint, std::vector<std::function<void(Book &)>>> mapBookFunction;
    std::map<xint, std::vector<std::function<void(BookBrief &)>>> mapBookBriefFunction;
    int threadCounts {};
    int threadLimit {};
    std::vector<std::function<void(xint &)>> browseEvents {};
};