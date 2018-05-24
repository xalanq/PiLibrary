// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QHBoxLayout>
#include <QIntValidator>
#include <QMessageBox>
#include <QVBoxLayout>

#include <client/thread/ThreadReturnBook.h>
#include <client/page/PageReturn.h>
#include <core/utils.h>

PageReturn::PageReturn(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    QWidget(parent) {

    lblBookid = new QLabel(this);
    lblUserid = new QLabel(this);

    editUserid = new QLineEdit(this);
    editBookid = new QLineEdit(this);

    btnReturn = new QPushButton(this);

    setUI();
    setConnection();
}

void PageReturn::slotReturn() {
    auto userid = editUserid->text().toInt();
    auto bookid = editBookid->text().toInt();
    auto thread = new ThreadReturnBook(userManager.getToken(), userid, bookid, this);
    connect(thread, &ThreadReturnBook::done, this, [this](const X::ErrorCode &ec) {
        QString text;
        if (ec != X::NoError)
            text = QString::fromStdString(X::what(ec));
        else
            text = tr("Successfully!");
        QMessageBox::information(this, tr("Return Result"), text, QMessageBox::Ok);
    });
    connect(thread, &ThreadReturnBook::finished, thread, &QObject::deleteLater);
    thread->start();
}

void PageReturn::setUI() {
    lblUserid->setText(tr("&Userid: "));
    lblUserid->setBuddy(editUserid);
    lblBookid->setText(tr("&Bookid: "));
    lblBookid->setBuddy(editBookid);
    btnReturn->setText(tr("&Return"));

    editUserid->setValidator(new QIntValidator(0, 2147483647));
    editBookid->setValidator(new QIntValidator(0, 2147483647));

    auto layout = new QVBoxLayout;

    auto layoutUserid = new QHBoxLayout;
    layoutUserid->addWidget(lblUserid);
    layoutUserid->addWidget(editUserid);
    layout->addLayout(layoutUserid);

    auto layoutBookid = new QHBoxLayout;
    layoutBookid->addWidget(lblBookid);
    layoutBookid->addWidget(editBookid);
    layout->addLayout(layoutBookid);

    layout->addWidget(btnReturn);
    layout->addStretch();

    setLayout(layout);
}

void PageReturn::setConnection() {
    connect(btnReturn, &QPushButton::clicked, this, &PageReturn::slotReturn);
}

