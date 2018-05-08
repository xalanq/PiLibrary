// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/usermanager.h>

class PageBrowse : public QWidget {
    Q_OBJECT

public:
    PageBrowse(UserManager &userManager, QWidget *parent = Q_NULLPTR);
    ~PageBrowse();

private:
    UserManager &userManager;
};
