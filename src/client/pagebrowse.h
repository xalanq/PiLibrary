// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/listwidgetbook.h>
#include <client/usermanager.h>
#include <client/xclient.h>

class PageBrowse : public QWidget {
    Q_OBJECT

public:
    PageBrowse(UserManager &userManager, QWidget *parent = Q_NULLPTR);

public slots:
    void slotEndGetBook(const X::ErrorCode, const ptree &pt);
    void slotEndGetNewBookList(const X::ErrorCode &ec, const ptree &pt);

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;

    ListWidgetBook *listWidgetBook;
};
