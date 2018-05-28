// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QSettings>
#include <QtWidgets/QMainWindow>

#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>
#include <client/widget/MainWidget.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();
    void go();
    void loadSetting();
    void saveSetting();

public slots:
    void logout();

private:
    void setUI();
    void setConnection();
    void login();

private:
    UserManager *userManager;
    BookManager *bookManager;
    MainWidget *mainWidget;
};
