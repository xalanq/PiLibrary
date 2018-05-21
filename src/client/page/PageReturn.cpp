// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/page/PageReturn.h>

PageReturn::PageReturn(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    QWidget(parent) {

}

