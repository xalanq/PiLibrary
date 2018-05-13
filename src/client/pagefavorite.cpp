// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/pagefavorite.h>

PageFavorite::PageFavorite(UserManager &userManager, QWidget *parent) :
    userManager(userManager),
    QWidget(parent) {
}

PageFavorite::~PageFavorite() {
}
