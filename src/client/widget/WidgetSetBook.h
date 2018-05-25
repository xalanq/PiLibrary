// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <vector>

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QStringList>
#include <QWidget>

#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>
#include <core/Resource.h>
#include <core/types.h>

class WidgetSetBook : public QWidget {
    Q_OBJECT

public:
    WidgetSetBook(UserManager &userManager, BookManager &bookManager, QWidget *parent = Q_NULLPTR);
    void getData();

public slots:
    void slotUpload();

private:
    void setUI();
    void setConnection();

protected:
    UserManager &userManager;
    BookManager &bookManager;

    QLabel *lblCover;
    QLabel *lblBookid;
    QLabel *lblTitle;
    QLabel *lblAuthor;
    QLabel *lblISBN;
    QLabel *lblPublisher;
    QLabel *lblIntroduction;
    QLabel *lblPriority;
    QLabel *lblPosition;
    QLabel *lblAmount;
    QLabel *lblKeepTime;
    QLabel *lblDay;
    QLabel *lblHour;
    QLabel *lblMinute;
    QLabel *lblMessage;

    QLineEdit *editBookid;
    QLineEdit *editTitle;
    QLineEdit *editAuthor;
    QLineEdit *editISBN;
    QLineEdit *editPublisher;
    QPlainTextEdit *editIntroduction;
    QComboBox *cbboxPriority;
    QLineEdit *editPosition;
    QLineEdit *editAmount;

    QPushButton *btnUploadCover;
    QPushButton *btnAdd;

    QSpinBox *spinDay;
    QSpinBox *spinHour;
    QSpinBox *spinMinute;

    QStringList priorityList;
    std::vector<X::xint> priorityInfo;

    X::ptree pt;
    Resource cover;
    X::xll beforeKeepTime;
};
