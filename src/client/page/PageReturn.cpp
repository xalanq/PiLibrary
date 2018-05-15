// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/page/PageReturn.h>

PageReturn::PageReturn(UserManager &userManager, BookBriefManager &bookBriefManager, QWidget *parent) :
    userManager(userManager),
    bookBriefManager(bookBriefManager),
    QWidget(parent) {

}

PageReturn::~PageReturn() {
}
