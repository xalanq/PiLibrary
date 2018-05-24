// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QDialog>
#include <QLabel>

class DialogRefresh : public QDialog {
    Q_OBJECT

public:
    DialogRefresh(QWidget *parent = Q_NULLPTR);

private:
    void setUI();

private:
    QLabel *lblRefresh;
};
