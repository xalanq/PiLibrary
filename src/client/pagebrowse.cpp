// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QVBoxLayout>

#include <core/book.h>
#include <client/pagebrowse.h>
#include <client/threadgetbookbrief.h>
#include <client/threadgetnewbooklist.h>

PageBrowse::PageBrowse(UserManager &userManager, BookBriefManager &bookBriefManager, QWidget *parent) :
    userManager(userManager),
    bookBriefManager(bookBriefManager),
    QWidget(parent) {

    listWidgetBook = new ListWidgetBook(this);

    setUI();
    setConnection();
}

void PageBrowse::slotGetBook(const X::ErrorCode &ec, const ptree &pt) {
    if (ec != X::NoError)
        return;
    auto &&book = BookBriefManager::parseBook(pt);
    bookBriefManager.add(book);
    listWidgetBook->add(bookBriefManager.get(book.getBookid()));
}

void PageBrowse::slotGetNewBookList(const X::ErrorCode &ec, const ptree &pt) {
    auto arr = pt.get_child("bookid");
    for (auto &&child : arr) {
        auto bookid = child.second.get_value<X::xint>();
        if (!bookBriefManager.has(bookid)) {
            auto thread = new ThreadGetBookBrief(userManager.getToken(), bookid, this);
            connect(thread, &ThreadGetBookBrief::done, this, &PageBrowse::slotGetBook);
            connect(thread, &ThreadGetBookBrief::finished, thread, &QObject::deleteLater);
            thread->start();
        } else {
            listWidgetBook->add(bookBriefManager.get(bookid));
        }
    }
}

void PageBrowse::setUI() {
    auto layout = new QVBoxLayout;
    layout->addWidget(listWidgetBook);
    setLayout(layout);
    
    auto thread = new ThreadGetNewBookList(userManager.getToken(), 15, this);
    connect(thread, &ThreadGetNewBookList::done, this, &PageBrowse::slotGetNewBookList);
    connect(thread, &ThreadGetNewBookList::finished, thread, &QObject::deleteLater);
    thread->start();
}

void PageBrowse::setConnection() {

}
