// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/listwidget/ListWidgetBrowseBook.h>
#include <client/manager/BookBriefManager.h>
#include <client/manager/UserManager.h>
#include <core/types.h>

class PageBrowse : public QWidget {
    Q_OBJECT

public:
    PageBrowse(UserManager &userManager, BookBriefManager &bookBriefManager, QWidget *parent = Q_NULLPTR);

public slots:
    void slotGetBookBrief(const X::ErrorCode &ec, const X::ptree &pt);
    void slotGetNewBookList(const X::ErrorCode &ec, const X::ptree &pt);

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;
    BookBriefManager &bookBriefManager;

    ListWidgetBrowseBook *listWidgetBrowseBook;
};
