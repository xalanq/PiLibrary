// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/pagesetting.h>

PageSetting::PageSetting(UserManager &userManager, QWidget *parent) :
    userManager(userManager),
    QWidget(parent) {

}

PageSetting::~PageSetting() {
}
