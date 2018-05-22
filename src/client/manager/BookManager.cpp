// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <memory>

#include <QSettings>

#include <client/thread/ThreadGetBook.h>
#include <client/manager/BookManager.h>


BookManager::BookManager(UserManager &userManager) :
    userManager(userManager) {

    QSettings setting;
    setting.beginGroup("Setting/Network");
    threadLimit = setting.value("threadNumber", 32).toInt();
    setting.endGroup();
}

bool BookManager::hasBook(const xint &bookid) const {
    return mapBook.find(bookid) != mapBook.end();
}

bool BookManager::hasBookBrief(const xint &bookid) const {
    return mapBookBrief.find(bookid) != mapBookBrief.end();
}

const Book& BookManager::getBook(const xint &bookid) const {
    try {
        return mapBook.at(bookid);
    } catch (std::exception &) {
        return Book::unknown();
    }
}

void BookManager::getBook(const xint &bookid, std::function<void(Book &)> f, bool update) {
    auto &v = mapBookFunction[bookid];
    if (v.size() != 0) {
        v.push_back(f);
    } else {
        if (!update) {
            auto it = mapBook.find(bookid);
            if (it != mapBook.end()) {
                f(it->second);
                return;
            }
        }
        v.push_back(f);
        if (threadCounts >= threadLimit)
            queueBook.push(bookid);
        else
            _getBook(bookid);
    }
}

const BookBrief& BookManager::getBookBrief(const xint &bookid) const {
    try {
        return mapBookBrief.at(bookid);
    } catch (std::exception &) {
        return BookBrief::unknown();
    }
}

void BookManager::getBookBrief(const xint &bookid, std::function<void(BookBrief &)> f, bool update) {
    auto &v = mapBookBriefFunction[bookid];
    if (v.size() != 0) {
        v.push_back(f);
    } else {
        if (!update) {
            auto it = mapBookBrief.find(bookid);
            if (it != mapBookBrief.end()) {
                f(it->second);
                return;
            }
        } 
        v.push_back(f);
        if (threadCounts >= threadLimit)
            queueBook.push(bookid);
        else
            _getBookBrief(bookid);
    }
}

void BookManager::updateStar(const xint &bookid, bool star) {
    auto it1 = mapBook.find(bookid);
    if (it1 != mapBook.end()) {
        it1->second.setStarCount(it1->second.getStarCount() + (star ? 1 : -1));
    }
    auto it2 = mapBookBrief.find(bookid);
    if (it2 != mapBookBrief.end()) {
        it2->second.setStarCount(it2->second.getStarCount() + (star ? 1 : -1));
    }
}

void BookManager::installBrowseEvent(std::function<void(xint &)> f) {
    browseEvents.push_back(f);
}

void BookManager::slotGetBook(const ErrorCode &ec, const ptree &pt, const xint &bookid, const Resource &cover) {
    auto &v = mapBookFunction[bookid];
    if (ec != X::NoError) {
        auto book = Book::unknown();
        for (auto f : v)
            f(book);
        v.clear();
    } else {
        try {
            auto &&book = Book::fromPtree(pt);
            book.setCover(cover);
            mapBook[bookid] = book;
            for (auto f : v)
                f(book);
            v.clear();
            emitBrowseEvents(bookid);
        } catch (std::exception &) { }
    }
    popThread();
}

void BookManager::slotGetBookBrief(const ErrorCode &ec, const ptree &pt, const xint &bookid, const Resource &cover) {
    auto &v = mapBookBriefFunction[bookid];
    if (ec != X::NoError) {
        auto book = BookBrief::unknown();
        for (auto f : v)
            f(book);
        v.clear();
    } else {
        try {
            auto &&book = BookBrief::fromPtree(pt);
            book.setCover(cover);
            mapBookBrief[bookid] = book;
            for (auto f : v)
                f(book);
            v.clear();
        } catch (std::exception &) { }
    }
    popThread();
}

void BookManager::_getBook(const xint &bookid) {
    ++threadCounts;
    auto thread = new ThreadGetBook(userManager.getToken(), bookid, false, this);
    connect(thread, &ThreadGetBook::done, this, 
            [this, bookid](const X::ErrorCode &ec, const X::ptree &pt, const Resource &cover) {
                slotGetBook(ec, pt, bookid, cover);
            }
    );
    connect(thread, &ThreadGetBook::finished, thread, &QObject::deleteLater);
    thread->start();
}

void BookManager::_getBookBrief(const xint &bookid) {
    ++threadCounts;
    auto thread = new ThreadGetBook(userManager.getToken(), bookid, true, this);
    connect(thread, &ThreadGetBook::done, this, 
            [this, bookid](const X::ErrorCode &ec, const X::ptree &pt, const Resource &cover) {
                slotGetBookBrief(ec, pt, bookid, cover);
            }
    );
    connect(thread, &ThreadGetBook::finished, thread, &QObject::deleteLater);
    thread->start();
}

void BookManager::popThread() {
    --threadCounts;
    if (threadCounts < threadLimit) {
        if (queueBook.size()) {
            _getBook(queueBook.front());
            queueBook.pop();
        } else if (queueBookBrief.size()) {
            _getBookBrief(queueBookBrief.front());
            queueBookBrief.pop();
        }
    }
}

void BookManager::emitBrowseEvents(const xint &bookid) {
    for (auto f : browseEvents)
        f(xint(bookid));
}
