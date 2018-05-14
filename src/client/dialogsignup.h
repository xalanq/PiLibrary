// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <client/networkthread.h>
#include <client/xclient.h>

class SignUpThread : public NetworkThread {
    Q_OBJECT

public:
    SignUpThread(const QString &username, const QString &nickname, const QString &password, const QString &email, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec);

private:
    void run() override;

    xstring username;
    xstring nickname;
    xstring password;
    xstring email;
};

class DialogSignUp : public QDialog {
    Q_OBJECT

public:
    DialogSignUp(QWidget *parent);

public slots:
    void slotSignUpBegin();
    void slotSignUpEnd(const int &ec);

private:
    void setUI();
    void setConnection();

    QLineEdit *editUsername;
    QLineEdit *editNickname;
    QLineEdit *editPassword;
    QLineEdit *editPasswordConfirm;
    QLineEdit *editEmail;

    QLabel *labelMessage;

    QDialogButtonBox *btns;
};
