// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <core/types.h>

class WidgetSearchBook : public QWidget {
    Q_OBJECT

public:
    WidgetSearchBook(QWidget *parent);

signals:
    void searchInfo(const X::ptree &pt);

public slots:
    void slotSearch();
    void slotMore();

private:
    void setUI();
    void setConnection();

private:
    QLabel *lblTitle;
    QLabel *lblAuthor;
    QLabel *lblIntroduction;
    QLabel *lblPosition;
    QLabel *lblBookid;
    QLabel *lblPublisher;
    QLabel *lblISBN;

    QLineEdit *editTitle;
    QLineEdit *editAuthor;
    QLineEdit *editIntroduction;
    QLineEdit *editPosition;
    QLineEdit *editBookid;
    QLineEdit *editPublisher;
    QLineEdit *editISBN;

    QCheckBox *regexTitle;
    QCheckBox *regexAuthor;
    QCheckBox *regexIntroduction;
    QCheckBox *regexPosition;
    QCheckBox *regexBookid;
    QCheckBox *regexPublisher;
    QCheckBox *regexISBN;

    QPushButton *btnSearch;
    QPushButton *btnMore;
};
