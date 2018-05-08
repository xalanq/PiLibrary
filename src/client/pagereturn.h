// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/usermanager.h>

class PageReturn : public QWidget {
    Q_OBJECT

public:
    PageReturn(UserManager &userManager, QWidget *parent = Q_NULLPTR);
    ~PageReturn();

private:
    UserManager &userManager;
};
