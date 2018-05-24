// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <memory>

#include <QSettings>

#include <client/thread/ThreadGetBook.h>
#include <client/thread/ThreadSetBook.h>
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

void BookManager::getBook(const xint &bookid, std::function<void(Book &)> f) {
    auto &v = mapGetBookFunction[bookid];
    if (v.size() != 0) {
        v.push_back(f);
    } else {
        auto it = mapBook.find(bookid);
        if (it != mapBook.end()) {
            f(it->second);
            return;
        }
        v.push_back(f);
        if (threadCounts >= threadLimit)
            queueGetBook.push(bookid);
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

void BookManager::getBookBrief(const xint &bookid, std::function<void(BookBrief &)> f) {
    auto &v = mapGetBookBriefFunction[bookid];
    if (v.size() != 0) {
        v.push_back(f);
    } else {
        auto it = mapBookBrief.find(bookid);
        if (it != mapBookBrief.end()) {
            f(it->second);
            return;
        } 
        v.push_back(f);
        if (threadCounts >= threadLimit)
            queueGetBook.push(bookid);
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

void BookManager::addBook(const ptree &pt, const Resource &cover, std::function<void(ErrorCode &)> f) {
    auto thread = new ThreadSetBook(userManager.getToken(), pt, cover, this);
    connect(thread, &ThreadSetBook::done, this, std::bind(&BookManager::slotAddBook, this, std::placeholders::_1, f));
    connect(thread, &ThreadSetBook::finished, thread, &QObject::deleteLater);
    thread->start();
}

void BookManager::updateBook(const ptree &pt, const Resource &cover, std::function<void(ErrorCode &)> f) {
    auto thread = new ThreadSetBook(userManager.getToken(), pt, cover, this);
    connect(thread, &ThreadSetBook::done, this, std::bind(&BookManager::slotUpdateBook, this, std::placeholders::_1, pt, cover, f));
    connect(thread, &ThreadSetBook::finished, thread, &QObject::deleteLater);
    thread->start();
}

void BookManager::installBrowseEvent(std::function<void(xint &)> f) {
    browseEvents.push_back(f);
}

void BookManager::refresh() {
    threadCounts = 0;

    for (auto &i : mapBook)
        i.second.cleanCover();
    mapBook.clear();
    for (; queueGetBook.size(); queueGetBook.pop());
    mapGetBookFunction.clear();

    for (auto &i : mapBookBrief)
        i.second.cleanCover();
    mapBookBrief.clear();
    for (; queueGetBookBrief.size(); queueGetBookBrief.pop());
    mapGetBookBriefFunction.clear();
}

void BookManager::slotGetBook(const ErrorCode &ec, const ptree &pt, const xint &bookid, const Resource &cover) {
    auto &v = mapGetBookFunction[bookid];
    if (ec != X::NoError) {
        auto book = Book::unknown();
        for (auto f : v)
            f(book);
        v.clear();
    } else {
        auto &book = mapBook[bookid] = Book::fromPtree(pt);
        book.setCover(cover);
        for (auto f : v)
            f(book);
        v.clear();
        emitBrowseEvents(bookid);
    }
    popThread();
}

void BookManager::slotGetBookBrief(const ErrorCode &ec, const ptree &pt, const xint &bookid, const Resource &cover) {
    auto &v = mapGetBookBriefFunction[bookid];
    if (ec != X::NoError) {
        auto book = BookBrief::unknown();
        for (auto f : v)
            f(book);
        v.clear();
    } else {
        auto &book = mapBookBrief[bookid] = BookBrief::fromPtree(pt);
        book.setCover(cover);
        for (auto f : v)
            f(book);
        v.clear();
    }
    popThread();
}

void BookManager::slotAddBook(const ErrorCode &ec, std::function<void(ErrorCode &)> f) {
    f(ErrorCode(ec));
}

void BookManager::slotUpdateBook(const ErrorCode &ec, const ptree &pt, const Resource &cover, std::function<void(ErrorCode &)> f) {
    if (ec == X::NoError) {
        auto bookid = pt.get<xint>("bookid");
        mapBook[bookid].updateFromPtree(pt).cleanCover().setCover(cover);
        mapBookBrief[bookid].updateFromPtree(pt).cleanCover().setCover(Resource::copy(cover));
    }
    f(ErrorCode(ec));
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
        if (queueGetBook.size()) {
            _getBook(queueGetBook.front());
            queueGetBook.pop();
        } else if (queueGetBookBrief.size()) {
            _getBookBrief(queueGetBookBrief.front());
            queueGetBookBrief.pop();
        }
    }
}

void BookManager::emitBrowseEvents(const xint &bookid) {
    for (auto f : browseEvents)
        f(xint(bookid));
}
