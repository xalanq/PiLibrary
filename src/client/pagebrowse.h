// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/listwidgetbook.h>
#include <client/networkthread.h>
#include <client/usermanager.h>
#include <client/xclient.h>

class GetBookThread : public NetworkThread {
    Q_OBJECT

public:
    GetBookThread(const xll &token, const xint &bookid, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec, const ptree &pt);

private:
    void run() override;

private:
    xll token;
    xint bookid;
};

class GetNewBookListThread : public NetworkThread {
    Q_OBJECT

public:
    GetNewBookListThread(const xll &token, const xint &number, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec,  const ptree &pt);

private:
    void run() override;

private:
    xll token;
    xint number;
};

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
