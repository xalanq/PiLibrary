// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QTabWidget>
#include <QWidget>

#include <client/listwidget/ListWidgetBrowseBook.h>
#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>
#include <client/widget/WidgetSearchBook.h>
#include <core/types.h>

class PageBrowse : public QWidget {
    Q_OBJECT

public:
    PageBrowse(UserManager &userManager, BookManager &bookManager, QWidget *parent = Q_NULLPTR);
    void updateStar(const X::xint &bookid, bool star);

signals:
    void signalReady();
    void signalModify();

public slots:
    void refresh();
    void slotSearch(const X::ptree &pt);

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;
    BookManager &bookManager;

    QTabWidget *tabWidget;
    ListWidgetBrowseBook *listWidgetNewBook;

    WidgetSearchBook *widgetSearchBook;
    ListWidgetBrowseBook *listWidgetSearchBook;
};
