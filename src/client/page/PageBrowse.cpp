// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QVBoxLayout>

#include <client/core/Book.h>
#include <client/page/PageBrowse.h>
#include <client/thread/ThreadGetNewBookList.h>
#include <client/thread/ThreadGetSearchBookList.h>

PageBrowse::PageBrowse(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    QWidget(parent) {

    tabWidget = new QTabWidget(this);
    listWidgetNewBook = new ListWidgetBrowseBook(userManager, bookManager, this);

    widgetSearchBook = new WidgetSearchBook(this);
    listWidgetSearchBook = new ListWidgetBrowseBook(userManager, bookManager, this);

    setUI();
    setConnection();
}

void PageBrowse::updateStar(const X::xint &bookid, bool star) {
    listWidgetNewBook->updateStar(bookid, star);
    listWidgetSearchBook->updateStar(bookid, star);
}

void PageBrowse::refresh() {
    listWidgetNewBook->clear();
    auto thread = new ThreadGetNewBookList(userManager.getToken(), 15, this);
    connect(thread, &ThreadGetNewBookList::done, listWidgetNewBook, &ListWidgetBrowseBook::slotGetBookList);
    connect(thread, &ThreadGetNewBookList::finished, thread, &QObject::deleteLater);
    thread->start();
}

void PageBrowse::slotSearch(const X::ptree &pt) {
    listWidgetSearchBook->clear();
    auto thread = new ThreadGetSearchBookList(userManager.getToken(), pt, this);
    connect(thread, &ThreadGetSearchBookList::done, listWidgetSearchBook, &ListWidgetBrowseBook::slotGetBookList);
    connect(thread, &ThreadGetSearchBookList::finished, thread, &QObject::deleteLater);
    thread->start();
}

void PageBrowse::setUI() {
    tabWidget->addTab(listWidgetNewBook, tr("Newest Book"));

    QWidget *w = new QWidget(this);
    auto layoutSearch = new QVBoxLayout;
    layoutSearch->addWidget(widgetSearchBook);
    layoutSearch->addWidget(listWidgetSearchBook);
    w->setLayout(layoutSearch);

    tabWidget->addTab(w, tr("Search Book"));

    auto layout = new QVBoxLayout;
    layout->addWidget(tabWidget);
    setLayout(layout);
}

void PageBrowse::setConnection() {
    connect(listWidgetNewBook,
            SIGNAL(signalModify()),
            this,
            SIGNAL(signalModify()));
    connect(listWidgetNewBook,
            SIGNAL(signalReady()),
            this,
            SIGNAL(signalReady()));
    connect(widgetSearchBook,
            &WidgetSearchBook::searchInfo,
            this,
            &PageBrowse::slotSearch);
}
