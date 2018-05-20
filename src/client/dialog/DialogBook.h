// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QDialog>
#include <QLabel>
#include <QPixmap>

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

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;
    BookManager &bookManager;
    X::xint bookid;

    QLabel *lblTitle;
    QLabel *lblAuthor;
    QLabel *lblIntroduction;
};
