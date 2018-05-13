// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/usermanager.h>

class PageFavorite : public QWidget {
    Q_OBJECT

public:
    PageFavorite(UserManager &userManager, QWidget *parent = Q_NULLPTR);
    ~PageFavorite();

private:
    UserManager &userManager;
};
