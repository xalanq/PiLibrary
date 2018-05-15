// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QSettings>
#include <QtWidgets/QMainWindow>

#include <client/manager/BookBriefManager.h>
#include <client/manager/UserManager.h>
#include <client/widget/MainWidget.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
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
