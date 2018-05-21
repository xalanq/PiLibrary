// copyright 2018 xalanq, chang-ran
// license: lgpl v3.0

#include <functional>

#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QVBoxLayout>

#include <client/dialog/DialogBook.h>
#include <client/dialog/DialogChooseTime.h>
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
    setWindowTitle(QString::fromStdString(book.getTitle()) + " - " + QString::fromStdString(book.getAuthor()));
    lblTitle->setText(tr("Title: ") + QString::fromStdString(book.getTitle()));
    lblAuthor->setText(tr("Author: ") + QString::fromStdString(book.getAuthor()));
    lblIntroduction->setText(tr("Introduction: ") + QString::fromStdString(book.getIntroduction()));

    auto maxKeepTime = book.getMaxKeepTime();
    X::xll d = maxKeepTime / 60 / 60 / 24;
    X::xll h = maxKeepTime / 60 / 60 - d * 24;
    X::xll m = maxKeepTime / 60 - d * 24 * 60 - h * 60;

    lblMaxKeepTime->setText(tr("Max keep time: ") + QString::number(d) + tr(" d ") + QString::number(h) + tr(" h ") + QString::number(m) + tr(" m "));
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
        userManager.starBook(bookid);
        btnStar->setText(strUnStar);
    } else {
        try {
            userManager.unStarBook(bookid);
        } catch (std::exception &) {}
        btnStar->setText(strStar);
    }
}

void DialogBook::slotBorrowBegin() {
    DialogChooseTime dialog(bookManager.getBookBrief(bookid).getMaxKeepTime(), this);
    if (dialog.exec() == QDialog::Accepted) {
        keepTime = dialog.getKeepTime();
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
    auto beginTime = time(0);
    auto endTime = beginTime + keepTime;
    userManager.borrowBook(bookid, beginTime, endTime);
    btnBorrow->setText(strBorrowed);
    btnBorrow->setDisabled(true);
}

void DialogBook::setUI() {
    setWindowTitle(tr("Book"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    if (userManager.isStaredBook(bookid))
        btnStar->setText(strUnStar);
    else
        btnStar->setText(strStar);

    btnBorrow->setText(strBorrow);
    if (userManager.isBorrowedBook(bookid)) {
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

