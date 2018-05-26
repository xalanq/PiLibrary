// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QMessageBox>

#include <client/page/PageAddBook.h>
#include <client/utils.h>

PageAddBook::PageAddBook(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    WidgetSetBook(userManager, bookManager, parent) {

    setUI();
    setConnection();
}

void PageAddBook::slotAdd() {
    WidgetSetBook::getData();
    bookManager.addBook(pt, cover, std::bind(&PageAddBook::slotAddEnd, this, std::placeholders::_1));
}

void PageAddBook::slotAddEnd(const X::ErrorCode &ec) {
    if (ec == X::NoError) {
        QMessageBox::information(this, tr("Add result"), tr("Successfully!"));
    } else {
        lblMessage->setText(X::what(ec));
        lblMessage->show();
    }
}

void PageAddBook::setUI() {
    lblBookid->hide();
    editBookid->setDisabled(true);
    editBookid->hide();
}

void PageAddBook::setConnection() {
    connect(btnAdd, &QPushButton::clicked, this, &PageAddBook::slotAdd);
}
