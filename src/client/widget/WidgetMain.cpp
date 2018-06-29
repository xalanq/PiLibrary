// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <functional>

#include <QEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QList>
#include <QString>
#include <QVBoxLayout>

#include <client/widget/WidgetMain.h>

WidgetMain::WidgetMain(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    QWidget(parent),
    userManager(userManager),
    bookManager(bookManager) {

    widgetHead = new WidgetHead(userManager, this);
    listWidget = new QListWidget(this);
    pageWidget = new QStackedWidget(this);
    scrollArea = new QScrollArea(this);
    dialogRefresh = new DialogRefresh(this);

    setUI();
    setConnection();
    refresh();
}

void WidgetMain::loadSetting() {

}

void WidgetMain::saveSetting() {

}

void WidgetMain::setEvents() {
    if (++eventCount == 3) {
        userManager.installStarEvent(std::bind(&BookManager::updateStar, &bookManager, std::placeholders::_1, std::placeholders::_2));
        userManager.installStarEvent(std::bind(&PageBrowse::updateStar, pageBrowse, std::placeholders::_1, std::placeholders::_2));
        userManager.installStarEvent(std::bind(&PageFavorite::updateStar, pageFavorite, std::placeholders::_1, std::placeholders::_2));
        userManager.installBorrowEvent(std::bind(&PageRecord::updateBorrow, pageRecord, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        bookManager.installBrowseEvent(std::bind(&PageRecord::updateBrowse, pageRecord, std::placeholders::_1));
        setDisabled(false);
        dialogRefresh->hide();
    }
}

void WidgetMain::refresh(bool force) {
    dialogRefresh->show();
    setDisabled(true);
    eventCount = 0;

    userManager.clearBorrowEvent();
    userManager.clearStarEvent();
    bookManager.clearBrowseEvent();
    if (force) {
        userManager.refresh();
        bookManager.refresh();
    }
    pageFavorite->refresh();
    pageRecord->refresh();
}

void WidgetMain::setUI() {
    initListWidget();
    initPageWidget();
    scrollArea->setWidget(pageWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto layoutLeft = new QVBoxLayout;
    layoutLeft->addWidget(widgetHead);
    layoutLeft->addWidget(listWidget);
    widgetHead->setMaximumHeight(150);
    widgetHead->setMaximumWidth(200);
    listWidget->setMaximumWidth(200);

    auto layout = new QHBoxLayout;

    layout->addLayout(layoutLeft);
    layout->addWidget(scrollArea);

    setLayout(layout);
}

void WidgetMain::setConnection() {
    connect(
        listWidget,
        &QListWidget::currentRowChanged,
        pageWidget,
        &QStackedWidget::setCurrentIndex
    );

    connect(
        pageFavorite,
        &PageFavorite::signalReady,
        pageBrowse,
        &PageBrowse::refresh
    );
    connect(
        pageBrowse,
        &PageBrowse::signalReady,
        this,
        &WidgetMain::setEvents
    );
    connect(
        pageFavorite,
        &PageFavorite::signalReady,
        this,
        &WidgetMain::setEvents
    );
    connect(
        pageRecord,
        &PageRecord::signalReady,
        this,
        &WidgetMain::setEvents
    );

    connect(pageBrowse, &PageBrowse::signalModify, this, std::bind(&WidgetMain::refresh, this, false));
    connect(pageFavorite, &PageFavorite::signalModify, this, std::bind(&WidgetMain::refresh, this, false));
    connect(pageRecord, &PageRecord::signalModify, this, std::bind(&WidgetMain::refresh, this, false));
    connect(pageSetting, &PageSetting::signalRefresh, this, std::bind(&WidgetMain::refresh, this, true));
    connect(pageSetting, &PageSetting::signalModifyUser, widgetHead, &WidgetHead::slotModify);
    connect(pageSetting, SIGNAL(signalLogout()), this, SIGNAL(signalLogout()));
}

void WidgetMain::initListWidget() {
    QStringList items;
    items.append(tr("Browse"));
    items.append(tr("Favorite"));
    items.append(tr("Record"));
    if (userManager.isAdminister())
        items.append(tr("Add Book"));
    if (userManager.isAdminister())
        items.append(tr("Administer"));
    items.append(tr("Setting"));
    items.append(tr("About"));

    listWidget->addItems(items);
    listWidget->setCurrentRow(0);
}

void WidgetMain::initPageWidget() {
    pageWidget->addWidget(pageBrowse = new PageBrowse(userManager, bookManager, this));
    pageWidget->addWidget(pageFavorite = new PageFavorite(userManager, bookManager, this));
    pageWidget->addWidget(pageRecord = new PageRecord(userManager, bookManager, this));
    if (userManager.isAdminister())
        pageWidget->addWidget(pageAddBook = new PageAddBook(userManager, bookManager, this));
    if (userManager.isAdminister())
        pageWidget->addWidget(pageAdminister = new PageAdminister(userManager, bookManager, this));
    pageWidget->addWidget(pageSetting = new PageSetting(userManager, this));
    pageWidget->addWidget(pageAbout = new PageAbout(this));

    pageWidget->setCurrentIndex(0);
}
