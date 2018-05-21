// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/manager/UserManager.h>

class PageSetting : public QWidget {
    Q_OBJECT

public:
    PageSetting(UserManager &userManager, QWidget *parent = Q_NULLPTR);

signals:
    void signalReady();

private:
    UserManager &userManager;
};
