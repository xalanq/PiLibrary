// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/pagebrowse.h>

PageBrowse::PageBrowse(UserManager &userManager, QWidget *parent) :
    userManager(userManager),
    QWidget(parent) {
}

PageBrowse::~PageBrowse() {
}
