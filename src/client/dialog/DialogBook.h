// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>

#include <client/core/Book.h>
#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>
#include <client/thread/ThreadGetBook.h>
#include <core/types.h>

class DialogBook : public QDialog {
    Q_OBJECT

public:
    DialogBook(UserManager &userManager, BookManager &bookManager, X::xint bookid, QWidget *parent = Q_NULLPTR);
    void setBook(const Book &book);

public slots:
    void slotStarBegin();
    void slotStarEnd(const X::ErrorCode &ec);
    void slotBorrowBegin();
    void slotBorrowEnd(const X::ErrorCode &ec);

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
    X::xint bookid;

    QLabel *lblTitle;
    QLabel *lblAuthor;
    QLabel *lblIntroduction;
    QLabel *lblMaxKeepTime;

    QPushButton *btnStar;
    QPushButton *btnBorrow;
};
