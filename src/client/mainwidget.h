// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QScrollArea>
#include <QSettings>
#include <QStackedWidget>
#include <QWidget>

#include <client/usermanager.h>
#include <client/widgethead.h>

class MainWidget : public QWidget {
    Q_OBJECT

public:
    MainWidget(UserManager &userManager, QWidget *parent = Q_NULLPTR);
    void loadSetting();
    void saveSetting();

private:
    void setUI();
    void setConnection();
    void initListWidget();
    void initPageWidget();

private:
    UserManager &userManager;
    QSettings setting {};

    WidgetHead *widgetHead;
    QListWidget *listWidget;
    QStackedWidget *pageWidget;
    QScrollArea *scrollArea;
};
