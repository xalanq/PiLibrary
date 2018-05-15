// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QLabel>
#include <QPushButton>
#include <QWidget>

class PageAbout : public QWidget {
    Q_OBJECT

public:
    PageAbout(QWidget *parent = Q_NULLPTR);

private:
    void setUI();
    void setConnection();

private:
    QLabel *label;
    QPushButton *btnUpdate;
};
