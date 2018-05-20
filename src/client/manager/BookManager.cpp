// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <memory>

#include <QSettings>

#include <client/thread/ThreadGetBook.h>
#include <client/thread/ThreadGetBookBrief.h>
#include <client/manager/BookManager.h>


BookManager::BookManager(UserManager &userManager) :
    userManager(userManager) {

    QSettings setting;
    setting.beginGroup("Setting/Network");
    threadLimit = setting.value("threadNumber", 32).toInt();
    setting.endGroup();
}

bool BookManager::hasBookBrief(const xint &bookid) const {
    return mapBookBrief.find(bookid) != mapBookBrief.end();
}

bool BookManager::hasBook(const xint &bookid) const {
    return mapBook.find(bookid) != mapBook.end();
}

const BookBrief& BookManager::getBookBrief(const xint &bookid) const {
    try {
        return mapBookBrief.at(bookid);
    } catch (std::exception &) {
        return BookBrief::unknown();
    }
}

void BookManager::getBookBrief(const xint &bookid, std::function<void(BookBrief &)> f, bool update) {
    if (!update) {
        auto it = mapBookBrief.find(bookid);
        if (it != mapBookBrief.end()) {
            f(it->second);
            return;
        }
    }
    if (threadCounts >= threadLimit) {
        queueBookBrief.push(make_pair(bookid, f));
    } else {
        _getBookBrief(bookid, f);
    }
}

const Book& BookManager::getBook(const xint &bookid) const {
    try {
        return mapBook.at(bookid);
    } catch (std::exception &) {
        return Book::unknown();
    }
}

void BookManager::getBook(const xint &bookid, std::function<void(Book &)> f, bool update) {
    if (!update) {
        auto it = mapBook.find(bookid);
        if (it != mapBook.end()) {
            f(it->second);
            return;
        }
    }
    if (threadCounts >= threadLimit) {
        queueBook.push(make_pair(bookid, f));
    } else {
        _getBook(bookid, f);
    }
}

void BookManager::slotGetBookBrief(const X::ErrorCode &ec, const X::ptree &pt, std::function<void(BookBrief &)> f) {
    popThread();
    if (ec != X::NoError) {
        auto book = BookBrief::unknown();
        f(book);
        return;
    }
    try {
        auto bookid = pt.get<xint>("bookid");
        BookBrief &&book = BookBrief::fromPtree(pt);
        mapBookBrief[bookid] = book;
        f(book);
    } catch (std::exception &) {
        ;
    }
}

void BookManager::slotGetBook(const X::ErrorCode &ec, const X::ptree &pt, std::function<void(Book &)> f) {
    popThread();
    if (ec != X::NoError) {
        auto book = Book::unknown();
        f(book);
        return;
    }
    try {
        auto bookid = pt.get<xint>("bookid");
        Book &&book = Book::fromPtree(pt);
        mapBook[bookid] = book;
        f(book);
    } catch (std::exception &) {
        ;
    }
}

void BookManager::_getBookBrief(const xint &bookid, std::function<void(BookBrief &)> f) {
    ++threadCounts;
    auto thread = new ThreadGetBookBrief(userManager.getToken(), bookid, this);
    connect(thread, &ThreadGetBookBrief::done, this, 
            [this, f](const X::ErrorCode &ec, const X::ptree &pt) {
                slotGetBookBrief(ec, pt, f);
            }
    );
    connect(thread, &ThreadGetBookBrief::finished, thread, &QObject::deleteLater);
    thread->start();
}

void BookManager::_getBook(const xint &bookid, std::function<void(Book &)> f) {
    ++threadCounts;
    auto thread = new ThreadGetBook(userManager.getToken(), bookid, this);
    connect(thread, &ThreadGetBook::done, this, 
            [this, f](const X::ErrorCode &ec, const X::ptree &pt) {
                slotGetBook(ec, pt, f);
            }
    );
    connect(thread, &ThreadGetBook::finished, thread, &QObject::deleteLater);
    thread->start();
}

void BookManager::popThread() {
    --threadCounts;
    if (threadCounts < threadLimit) {
        if (queueBook.size()) {
            auto &&tmp = queueBook.front();
            _getBook(tmp.first, tmp.second);
            queueBook.pop();
        } else if (queueBookBrief.size()) {
            auto &&tmp = queueBookBrief.front();
            _getBookBrief(tmp.first, tmp.second);
            queueBookBrief.pop();
        }
    }
}
