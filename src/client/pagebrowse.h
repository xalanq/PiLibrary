// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/bookbriefmanager.h>
#include <client/listwidgetbook.h>
#include <client/usermanager.h>
#include <client/xclient.h>

class PageBrowse : public QWidget {
    Q_OBJECT

public:
    PageBrowse(UserManager &userManager, BookBriefManager &bookBriefManager, QWidget *parent = Q_NULLPTR);

public slots:
    void slotGetBook(const X::ErrorCode &ec, const ptree &pt);
    void slotGetNewBookList(const X::ErrorCode &ec, const ptree &pt);

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;
    BookBriefManager &bookBriefManager;

    ListWidgetBook *listWidgetBook;
};
