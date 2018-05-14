// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QVBoxLayout>

#include <core/book.h>
#include <client/pagebrowse.h>
#include <client/threadgetbook.h>
#include <client/threadgetnewbooklist.h>

PageBrowse::PageBrowse(UserManager &userManager, QWidget *parent) :
    userManager(userManager),
    QWidget(parent) {

    listWidgetBook = new ListWidgetBook(this);

    setUI();
    setConnection();
}

void PageBrowse::slotEndGetBook(const X::ErrorCode, const ptree &pt) {
    Book book;
    book.setTitle(pt.get("title", "no title"));
    listWidgetBook->addBook(book);
}

void PageBrowse::slotEndGetNewBookList(const X::ErrorCode &ec, const ptree &pt) {
    auto arr = pt.get_child("bookid");
    for (auto &&child : arr) {
        auto bookid = child.second.get_value<X::xint>();
        auto thread = new ThreadGetBook(userManager.getToken(), bookid, this);
        connect(thread, &ThreadGetBook::done, this, &PageBrowse::slotEndGetBook);
        connect(thread, &ThreadGetBook::finished, thread, &QObject::deleteLater);
        thread->start();
    }
}

void PageBrowse::setUI() {
    auto layout = new QVBoxLayout;
    layout->addWidget(listWidgetBook);
    setLayout(layout);
    
    auto thread = new ThreadGetNewBookList(userManager.getToken(), 15, this);
    connect(thread, &ThreadGetNewBookList::done, this, &PageBrowse::slotEndGetNewBookList);
    connect(thread, &ThreadGetNewBookList::finished, thread, &QObject::deleteLater);
    thread->start();
}

void PageBrowse::setConnection() {

}
