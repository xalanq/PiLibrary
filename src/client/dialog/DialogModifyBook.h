// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QDialog>

#include <client/core/Book.h>
#include <client/widget/WidgetSetBook.h>

class WidgetModifyBook : public WidgetSetBook {
    Q_OBJECT

public:
    WidgetModifyBook(UserManager &userManager, const Book &book, QWidget *parent = Q_NULLPTR);

private:
    void setUI();

private:
    const Book &book;
};

class DialogModifyBook : public QDialog {
    Q_OBJECT

public:
    DialogModifyBook(UserManager &userManager, const Book &book, QWidget *parent = Q_NULLPTR);

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;
    const Book &book;

    WidgetModifyBook *w;
};
