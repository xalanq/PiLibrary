// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QSettings>
#include <QtWidgets/QMainWindow>

#include <client/bookbriefmanager.h>
#include <client/mainwidget.h>
#include <client/usermanager.h>
#include <client/xclient.h>

class Client : public QMainWindow {
    Q_OBJECT

public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    Client(QWidget *parent = Q_NULLPTR);
    void loadSetting();
    void saveSetting();

private:
    void setUI();
    void setConnection();
    void login();

private:
    UserManager userManager;
    BookBriefManager bookBriefManager;
    MainWidget *mainWidget;
    QSettings setting {};
};
