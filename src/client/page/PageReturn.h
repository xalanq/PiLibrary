// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>

class PageReturn : public QWidget {
    Q_OBJECT

public:
    PageReturn(UserManager &userManager, BookManager &bookManager, QWidget *parent = Q_NULLPTR);

signals:
    void signalReady();

public slots:
    void slotReturn();

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;
    BookManager &bookManager;

    QLabel *lblUserid;
    QLabel *lblBookid;

    QLineEdit *editUserid;
    QLineEdit *editBookid;

    QPushButton *btnReturn;
};
