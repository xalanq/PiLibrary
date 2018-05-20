// copyright 2018 xalanq, chang-ran
// license: lgpl v3.0

#include <functional>

#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QVBoxLayout>

#include <client/dialog/DialogBook.h>
#include <client/thread/ThreadBorrowBook.h>
#include <client/thread/ThreadStarBook.h>
#include <client/thread/ThreadUnStarBook.h>
#include <core/utils.h>

DialogBook::DialogBook(UserManager &userManager, BookManager &bookManager, X::xint bookid, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    bookid(bookid),
    QDialog(parent) {

    strStar = tr("Star");
    strUnStar = tr("Unstar");
    strBorrow = tr("Borrow");
    strBorrowed = tr("Borrowed");

    lblTitle = new QLabel(this);
    lblAuthor = new QLabel(this);
    lblIntroduction = new QLabel(this);
    lblMaxKeepTime = new QLabel(this);

    btnStar = new QPushButton(this);
    btnBorrow = new QPushButton(this);

    setUI();
    setConnection();
}

void DialogBook::setBook(const Book &book) {
    lblTitle->setText(tr("Title: ") + QString::fromStdString(book.getTitle()));
    lblAuthor->setText(tr("Author: ") + QString::fromStdString(book.getAuthor()));
    lblIntroduction->setText(tr("Introduction: ") + QString::fromStdString(book.getIntroduction()));
    lblMaxKeepTime->setText(tr("Max Keep Time: ") + QString::number((double)book.getMaxKeepTime() / 1000 / 60, 'g', 2) + tr(" hours"));
}

void DialogBook::slotStarBegin() {
    if (btnStar->text() == strStar) {
        auto thread = new ThreadStarBook(userManager.getToken(), bookid, this);
        connect(thread, &ThreadStarBook::done, this, &DialogBook::slotStarEnd);
        connect(thread, &ThreadStarBook::finished, thread, &QObject::deleteLater);
        thread->start();
    } else {
        auto thread = new ThreadUnStarBook(userManager.getToken(), bookid, this);
        connect(thread, &ThreadUnStarBook::done, this, &DialogBook::slotStarEnd);
        connect(thread, &ThreadUnStarBook::finished, thread, &QObject::deleteLater);
        thread->start();
    }
}

void DialogBook::slotStarEnd(const X::ErrorCode &ec) {
    if (ec != X::NoError) {
        QMessageBox::critical(
            this,
            tr("Failed to star"),
            QString::fromStdString(X::what(ec))
        );
        return;
    }
    if (btnStar->text() == strStar) {
        userManager.getStarBooks().insert(bookid);
        btnStar->setText(strUnStar);
    } else {
        try {
            userManager.getStarBooks().erase(bookid);
        } catch (std::exception &) {}
        btnStar->setText(strStar);
    }
}

void DialogBook::slotBorrowBegin() {
    auto ret = QMessageBox::information(
        this,
        tr("Borrow"),
        tr("Ensure you want to borrow this book. \nNote: you can not return this book yourself."),
        QMessageBox::Ok,
        QMessageBox::Cancel
    );
    if (ret == QMessageBox::Ok) {
        auto keepTime = bookManager.getBookBrief(bookid).getMaxKeepTime();
        auto thread = new ThreadBorrowBook(userManager.getToken(), bookid, keepTime, this);
        connect(thread, &ThreadBorrowBook::done, this, &DialogBook::slotBorrowEnd);
        connect(thread, &ThreadBorrowBook::finished, thread, &QObject::deleteLater);
        thread->start();
    }
}

void DialogBook::slotBorrowEnd(const X::ErrorCode &ec) {
    if (ec != X::NoError) {
        QMessageBox::critical(
            this,
            tr("Failed to borrow"),
            QString::fromStdString(X::what(ec))
        );
        return;
    }
    userManager.getKeepBooks().insert(bookid);
    btnBorrow->setText(strBorrowed);
    btnBorrow->setDisabled(true);
}

void DialogBook::setUI() {
    if (userManager.getStarBooks().find(bookid) != userManager.getStarBooks().end())
        btnStar->setText(strUnStar);
    else
        btnStar->setText(strStar);

    btnBorrow->setText(strBorrow);
    if (userManager.getKeepBooks().find(bookid) != userManager.getKeepBooks().end()) {
        btnBorrow->setText(strBorrowed);
        btnBorrow->setDisabled(true);
    }

    auto layoutRight = new QVBoxLayout;
    layoutRight->addWidget(lblTitle);
    layoutRight->addWidget(lblAuthor);
    layoutRight->addWidget(lblIntroduction);
    layoutRight->addWidget(lblMaxKeepTime);

    auto layoutButton = new QHBoxLayout;

    layoutButton->addWidget(btnStar);
    layoutButton->addWidget(btnBorrow);

    auto layout = new QVBoxLayout;
    layout->addLayout(layoutRight);
    layout->addLayout(layoutButton);

    setLayout(layout);

    bookManager.getBook(bookid, std::bind(&DialogBook::setBook, this, std::placeholders::_1));
}

void DialogBook::setConnection() {
    connect(btnStar,
            &QPushButton::clicked,
            this,
            &DialogBook::slotStarBegin);
    connect(btnBorrow,
            &QPushButton::clicked,
            this,
            &DialogBook::slotBorrowBegin);

}

