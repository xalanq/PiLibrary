// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <QWidget>

#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>

class PageAdminister : public QWidget {
    Q_OBJECT

public:
    PageAdminister(UserManager &userManager, BookManager &bookManager, QWidget *parent = Q_NULLPTR);

signals:
    void signalReady();

public slots:
    void slotReturn();
    void slotSetPriority();

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;
    BookManager &bookManager;

    QLabel *lblReturnUserid;
    QLabel *lblReturnBookid;
    QLineEdit *editReturnUserid;
    QLineEdit *editReturnBookid;
    QPushButton *btnReturn;

    QLabel *lblSetPriorityUserid;
    QLabel *lblSetPriority;
    QLineEdit *editSetPriorityUserid;
    QComboBox *cbboxSetPriority;
    QPushButton *btnSetPriority;

    QStringList priorityList;
    std::vector<X::xint> priorityInfo;
};
