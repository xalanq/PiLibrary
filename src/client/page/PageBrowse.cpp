// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <functional>

#include <QVBoxLayout>

#include <client/core/Book.h>
#include <client/dialog/DialogBook.h>
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

void PageBrowse::slotGetNewBookList(const X::ErrorCode &ec, const X::ptree &pt) {
    if (ec != X::NoError)
        return;
    auto arr = pt.get_child("bookid");
    for (auto &&child : arr) {
        auto bookid = child.second.get_value<X::xint>();
        bookManager.getBookBrief(bookid, std::bind(&ListWidgetBrowseBook::add, listWidgetBrowseBook, std::placeholders::_1));
    }
}

void PageBrowse::slotItemClicked(QListWidgetItem *item) {
    auto x = dynamic_cast<ListWidgetItemBook *> (item);
    DialogBook dialog(userManager, bookManager, x->getBook().getBookid());
    dialog.exec();
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
    connect(listWidgetBrowseBook,
            &ListWidgetBrowseBook::itemDoubleClicked,
            this,
            &PageBrowse::slotItemClicked);
}
