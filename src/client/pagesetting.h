// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/usermanager.h>

class PageSetting : public QWidget {
    Q_OBJECT

public:
    PageSetting(UserManager &userManager, QWidget *parent = Q_NULLPTR);
    ~PageSetting();

private:
    UserManager &userManager;
};
