// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <boost/asio.hpp>

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QThread>

#include <client/xclient.h>

class SignUpThread : public QThread {
    Q_OBJECT

public:
    SignUpThread(const QString &username, const QString &nickname, const QString &password, const QString &email, QObject *parent = Q_NULLPTR);

signals:
    void done(const int &ec);

private:
    void run() override;

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint ep;
    X::xstring username;
    X::xstring nickname;
    X::xstring password;
    X::xstring email;
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
