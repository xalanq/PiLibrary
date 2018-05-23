// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/page/PageAddBook.h>

PageAddBook::PageAddBook(UserManager &userManager, QWidget *parent) :
    WidgetSetBook(userManager, parent) {

    setUI();
}

void PageAddBook::setUI() {
    lblBookid->hide();
    editBookid->setDisabled(true);
    editBookid->hide();
}
