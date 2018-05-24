// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QLabel>
#include <QWidget>

#include <client/manager/UserManager.h>

class WidgetHead : public QWidget {
    Q_OBJECT

public:
    WidgetHead(UserManager &userManager, QWidget *parent = Q_NULLPTR);

public slots:
    void slotModify();

private:
    void setUI();

private:
    UserManager &userManager;

    QLabel *lblNickname;
};