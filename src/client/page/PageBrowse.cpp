// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <functional>
#include <vector>

#include <QVBoxLayout>

#include <client/core/Book.h>
#include <client/dialog/DialogBook.h>
#include <client/page/PageBrowse.h>
#include <client/thread/ThreadGetNewBookList.h>

PageBrowse::PageBrowse(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    QWidget(parent) {

    listWidgetBrowseBook = new ListWidgetBrowseBook(this);

    setUI();
    setConnection();
}

void PageBrowse::updateStar(const X::xint &bookid, bool star) {
    auto it = books.find(bookid);
    if (it != books.end())
        bookManager.getBookBrief(bookid, std::bind(&ListWidgetBrowseBook::update, listWidgetBrowseBook, std::placeholders::_1, star, it->second));
}

void PageBrowse::slotGetNewBookList(const X::ErrorCode &ec, const X::ptree &pt) {
    if (ec != X::NoError)
        return;
    auto arr = pt.get_child("bookid");
    books.clear();
    std::vector<X::xint> l;
    for (auto &&child : arr) {
        auto bookid = child.second.get_value<X::xint>();
        l.push_back(bookid);
    }
    int tot = int(l.size());
    for (int i = 0; i < tot; ++i)
        listWidgetBrowseBook->add(BookBrief::unknown(), 0);
    for (int i = 0; i < tot; ++i) {
        auto bookid = l[i];
        auto pos = tot - 1 - i;
        bookManager.getBookBrief(bookid, std::bind(&ListWidgetBrowseBook::update, listWidgetBrowseBook, std::placeholders::_1, userManager.isStaredBook(bookid), pos));
        books[bookid] = pos;
    }
    emit signalReady();
}

void PageBrowse::slotItemClicked(QListWidgetItem *item) {
    auto x = dynamic_cast<ListWidgetItemBook *> (item);
    auto dialog = new DialogBook(userManager, bookManager, x->getBook().getBookid(), this);
    connect(dialog, SIGNAL(signalModify()), this, SIGNAL(signalModify()));
    dialog->show();
}

void PageBrowse::refresh() {
    listWidgetBrowseBook->clear();
    auto thread = new ThreadGetNewBookList(userManager.getToken(), 15, this);
    connect(thread, &ThreadGetNewBookList::done, this, &PageBrowse::slotGetNewBookList);
    connect(thread, &ThreadGetNewBookList::finished, thread, &QObject::deleteLater);
    thread->start();
}

void PageBrowse::setUI() {
    auto layout = new QVBoxLayout;
    layout->addWidget(listWidgetBrowseBook);
    setLayout(layout);
}

void PageBrowse::setConnection() {
    connect(listWidgetBrowseBook,
            &ListWidgetBrowseBook::itemDoubleClicked,
            this,
            &PageBrowse::slotItemClicked);
}
