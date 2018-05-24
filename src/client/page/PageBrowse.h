// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <map>

#include <QWidget>

#include <client/listwidget/ListWidgetBrowseBook.h>
#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>
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
    void slotGetNewBookList(const X::ErrorCode &ec, const X::ptree &pt);
    void slotItemClicked(QListWidgetItem *item);

    void refresh();

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;
    BookManager &bookManager;

    ListWidgetBrowseBook *listWidgetBrowseBook;

    std::map<X::xint, int> books;
};
