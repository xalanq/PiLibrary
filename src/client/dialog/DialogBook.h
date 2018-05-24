// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QDateTimeEdit>

#include <client/core/Book.h>
#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>
#include <client/thread/ThreadGetBook.h>
#include <core/types.h>

class DialogBook : public QDialog {
    Q_OBJECT

public:
    DialogBook(UserManager &userManager, BookManager &bookManager, const X::xint &bookid, QWidget *parent = Q_NULLPTR);
    void setBook(const Book &book);

signals:
    void signalModify();

public slots:
    void slotStarBegin();
    void slotStarEnd(const X::ErrorCode &ec);
    void slotBorrowBegin();
    void slotBorrowEnd(const X::ErrorCode &ec);
    void slotModify();
    void slotMore();

private:
    void setUI();
    void setConnection();

private:
    QString strStar;
    QString strUnStar;
    QString strBorrow;
    QString strBorrowed;

    UserManager &userManager;
    BookManager &bookManager;
    Book const * bookPtr;
    X::xll keepTime {};

    QLabel *lblCover;
    QLabel *lblTitle;
    QLabel *lblAuthor;
    QLabel *lblIntroduction;
    QLabel *lblPosition;
    QLabel *lblMaxKeepTime;

    QLabel *lblBookid;
    QLabel *lblPublisher;
    QLabel *lblISBN;
    QLabel *lblAmount;

    QPushButton *btnStar;
    QPushButton *btnBorrow;
    QPushButton *btnModify;
    QPushButton *btnMore;
};
