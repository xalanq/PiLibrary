// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QDialog>

#include <client/core/Book.h>
#include <client/widget/WidgetSetBook.h>

class WidgetModifyBook : public WidgetSetBook {
    Q_OBJECT

public:
    WidgetModifyBook(UserManager &userManager, BookManager &bookManager, const Book &book, QWidget *parent = Q_NULLPTR);

signals:
    void signalModify();
   
public slots:
    void slotModify();
    void slotModifyEnd(const X::ErrorCode &ec);

private:
    void setUI();
    void setConnection();

private:
    const Book &book;
};

class DialogModifyBook : public QDialog {
    Q_OBJECT

public:
    DialogModifyBook(UserManager &userManager, BookManager &bookManager, const Book &book, QWidget *parent = Q_NULLPTR);

signals:
    void signalModify();

private:
    void setUI();
    void setConnection();

private:
    WidgetModifyBook *w;
};
