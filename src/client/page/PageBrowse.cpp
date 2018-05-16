// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QVBoxLayout>

#include <client/core/Book.h>
#include <client/page/PageBrowse.h>
#include <client/thread/ThreadGetBookBrief.h>
#include <client/thread/ThreadGetNewBookList.h>

PageBrowse::PageBrowse(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    QWidget(parent) {

    listWidgetBrowseBook = new ListWidgetBrowseBook(this);

    setUI();
    setConnection();
}

void PageBrowse::slotGetBookBrief(const X::ErrorCode &ec, const X::ptree &pt) {
    if (ec != X::NoError)
        return;
    auto &&book = BookBrief::fromPtree(pt);
    bookManager.addBookBrief(book);
    listWidgetBrowseBook->add(bookManager.getBookBrief(book.getBookid()));
}

void PageBrowse::slotGetNewBookList(const X::ErrorCode &ec, const X::ptree &pt) {
    auto arr = pt.get_child("bookid");
    for (auto &&child : arr) {
        auto bookid = child.second.get_value<X::xint>();
        if (!bookManager.hasBookBrief(bookid)) {
            auto thread = new ThreadGetBookBrief(userManager.getToken(), bookid, this);
            connect(thread, &ThreadGetBookBrief::done, this, &PageBrowse::slotGetBookBrief);
            connect(thread, &ThreadGetBookBrief::finished, thread, &QObject::deleteLater);
            thread->start();
        } else {
            listWidgetBrowseBook->add(bookManager.getBookBrief(bookid));
        }
    }
}

void PageBrowse::setUI() {
    auto layout = new QVBoxLayout;
    layout->addWidget(listWidgetBrowseBook);
    setLayout(layout);
    
    auto thread = new ThreadGetNewBookList(userManager.getToken(), 15, this);
    connect(thread, &ThreadGetNewBookList::done, this, &PageBrowse::slotGetNewBookList);
    connect(thread, &ThreadGetNewBookList::finished, thread, &QObject::deleteLater);
    thread->start();
}

void PageBrowse::setConnection() {

}
