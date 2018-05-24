// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QScrollArea>
#include <QSettings>
#include <QStackedWidget>
#include <QWidget>

#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>
#include <client/page/PageAbout.h>
#include <client/page/PageAddBook.h>
#include <client/page/PageBrowse.h>
#include <client/page/PageFavorite.h>
#include <client/page/PageRecord.h>
#include <client/page/PageReturn.h>
#include <client/page/PageSetting.h>
#include <client/widget/WidgetHead.h>

class MainWidget : public QWidget {
    Q_OBJECT

public:
    MainWidget(UserManager &userManager, BookManager &bookManager, QWidget *parent = Q_NULLPTR);
    void loadSetting();
    void saveSetting();

signals:
    void signalLogout();

public slots:
    void setEvents();
    void refresh(bool force = true);

private:
    void setUI();
    void setConnection();
    void initListWidget();
    void initPageWidget();

private:
    UserManager &userManager;
    BookManager &bookManager;

    QSettings setting;

    WidgetHead *widgetHead;
    QListWidget *listWidget;
    QStackedWidget *pageWidget;
    QScrollArea *scrollArea;

    PageAbout *pageAbout;
    PageAddBook *pageAddBook;
    PageBrowse *pageBrowse;
    PageFavorite *pageFavorite;
    PageRecord *pageRecord;
    PageReturn *pageReturn;
    PageSetting *pageSetting;

    int eventCount;
};
