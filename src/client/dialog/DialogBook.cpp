// copyright 2018 xalanq, chang-ran
// license: lgpl v3.0

#include <functional>

#include <QHBoxLayout>
#include <QPainter>
#include <QVBoxLayout>

#include <client/dialog/DialogBook.h>
#include <client/thread/ThreadGetBook.h>

DialogBook::DialogBook(UserManager &userManager, BookManager &bookManager, X::xint bookid, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    bookid(bookid),
    QDialog(parent) {

    setUI();
    setConnection();
}

void DialogBook::setBook(const Book &book) {
    lblTitle->setText(tr("Title: ") + QString::fromStdString(book.getTitle()));
    lblAuthor->setText(tr("Author: ") + QString::fromStdString(book.getAuthor()));
    lblIntroduction->setText(tr("Introduction: ") + QString::fromStdString(book.getIntroduction()));
}

void DialogBook::setUI() {
    lblTitle = new QLabel(this);
    lblAuthor = new QLabel(this);
    lblIntroduction = new QLabel(this);


    auto layoutRight = new QVBoxLayout;
    layoutRight->addWidget(lblTitle);
    layoutRight->addWidget(lblAuthor);
    layoutRight->addWidget(lblIntroduction);

    auto layout = new QHBoxLayout;
    layout->addLayout(layoutRight);

    setLayout(layout);

    bookManager.getBook(bookid, std::bind(&DialogBook::setBook, this, std::placeholders::_1));
}

void DialogBook::setConnection() {

}

