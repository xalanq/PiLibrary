// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include <client/manager/UserManager.h>

class WidgetHead : public QWidget {
    Q_OBJECT

public:
    WidgetHead(UserManager &userManager, QWidget *parent = Q_NULLPTR);
    ~WidgetHead();

public slots:
    void slotModify();

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;

    QLabel *lblNickname;
    QPushButton *btnModify;
};