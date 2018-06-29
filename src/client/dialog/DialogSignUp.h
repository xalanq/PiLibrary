// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <core/types.h>

class DialogSignUp : public QDialog {
    Q_OBJECT

public:
    DialogSignUp(QWidget *parent);

public slots:
    void slotSignUpBegin();
    void slotSignUpEnd(const X::ErrorCode &ec);

private:
    void setUI();
    void setConnection();

private:
    QLineEdit *editUsername;
    QLineEdit *editNickname;
    QLineEdit *editPassword;
    QLineEdit *editPasswordConfirm;
    QLineEdit *editEmail;

    QLabel *lblMessage;

    QDialogButtonBox *btns;
};
