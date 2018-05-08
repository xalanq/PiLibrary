// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/usermanager.h>

class PageRecord : public QWidget {
    Q_OBJECT

public:
    PageRecord(UserManager &userManager, QWidget *parent = Q_NULLPTR);
    ~PageRecord();

private:
    UserManager &userManager;
};
