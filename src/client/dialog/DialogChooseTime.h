// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QDialog>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSpinBox>

#include <core/types.h>

class DialogChooseTime : public QDialog {
    Q_OBJECT

public:
    DialogChooseTime(const X::xll &maxKeepTime, QWidget *parent = Q_NULLPTR);
    X::xll getKeepTime() const;

public slots:
    void slotGo();
    void slotChanged(int);

private:
    void setUI();
    void setConnection();

private:
    QLabel *lblBeginTime;
    QLabel *lblEndTime;
    QLabel *lblMaxKeepTime;
    QLabel *lblWarning;
    QLabel *lblError;
    QLabel *lblDay;
    QLabel *lblHour;
    QLabel *lblMinute;

    QSpinBox *spinDay;
    QSpinBox *spinHour;
    QSpinBox *spinMinute;

    QDialogButtonBox *btns;
    
    X::xll beginTime;
    X::xll keepTime;
    X::xll maxKeepTime;
};
