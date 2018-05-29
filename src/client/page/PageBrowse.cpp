// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QVBoxLayout>

#include <client/core/Book.h>
#include <client/page/PageBrowse.h>
#include <client/thread/ThreadGetNewBookList.h>
#include <client/thread/ThreadGetTopBookList.h>
#include <client/thread/ThreadGetSearchBookList.h>

PageBrowse::PageBrowse(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    QWidget(parent) {

    tabWidget = new QTabWidget(this);

    listWidgetNewBook = new ListWidgetBrowseBook(userManager, bookManager, this);

    listWidgetTopBook = new ListWidgetBrowseBook(userManager, bookManager, this);

    widgetSearchBook = new WidgetSearchBook(this);
    listWidgetSearchBook = new ListWidgetBrowseBook(userManager, bookManager, this);

    setUI();
    setConnection();
}

void PageBrowse::updateStar(const X::xint &bookid, bool star) {
    listWidgetNewBook->updateStar(bookid, star);
    listWidgetTopBook->updateStar(bookid, star);
    listWidgetSearchBook->updateStar(bookid, star);
}

void PageBrowse::slotReady() {
    if (++readyCount == 2) {
        emit signalReady();
    }
}

void PageBrowse::refresh() {
    readyCount = 0;
    {
        listWidgetNewBook->clear();
        auto thread = new ThreadGetNewBookList(userManager.getToken(), 15, this);
        connect(thread, &ThreadGetNewBookList::done, listWidgetNewBook, std::bind(&ListWidgetBrowseBook::slotGetBookList, listWidgetNewBook, std::placeholders::_1, std::placeholders::_2, false));
        connect(thread, &ThreadGetNewBookList::finished, thread, &QObject::deleteLater);
        thread->start();
    }
    {
        listWidgetTopBook->clear();
        auto thread = new ThreadGetTopBookList(userManager.getToken(), 15, this);
        connect(thread, &ThreadGetTopBookList::done, listWidgetTopBook, std::bind(&ListWidgetBrowseBook::slotGetBookList, listWidgetTopBook, std::placeholders::_1, std::placeholders::_2, true));
        connect(thread, &ThreadGetTopBookList::finished, thread, &QObject::deleteLater);
        thread->start();
    }
}

void PageBrowse::slotSearch(const X::ptree &pt) {
    listWidgetSearchBook->clear();
    auto thread = new ThreadGetSearchBookList(userManager.getToken(), pt, this);
    connect(thread, &ThreadGetSearchBookList::done, listWidgetSearchBook, std::bind(&ListWidgetBrowseBook::slotGetBookList, listWidgetSearchBook, std::placeholders::_1, std::placeholders::_2, false));
    connect(thread, &ThreadGetSearchBookList::finished, thread, &QObject::deleteLater);
    thread->start();
}

void PageBrowse::setUI() {
    tabWidget->addTab(listWidgetNewBook, tr("Newest Book"));
    tabWidget->addTab(listWidgetTopBook, tr("Top Book"));

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
    connect(listWidgetTopBook,
            SIGNAL(signalModify()),
            this,
            SIGNAL(signalModify()));
    connect(listWidgetSearchBook,
            SIGNAL(signalModify()),
            this,
            SIGNAL(signalModify()));
    connect(listWidgetNewBook,
            SIGNAL(signalReady()),
            this,
            SLOT(slotReady()));
    connect(listWidgetTopBook,
            SIGNAL(signalReady()),
            this,
            SLOT(slotReady()));
    connect(widgetSearchBook,
            &WidgetSearchBook::searchInfo,
            this,
            &PageBrowse::slotSearch);
}
