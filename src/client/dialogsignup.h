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
    SignUpThread(const X::string &username, const X::string &nickname, const X::string &password, const X::string &email, QObject *parent = Q_NULLPTR);

signals:
    void done(const int &ec);

private:
    void run() override;

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint ep;
    X::string username;
    X::string nickname;
    X::string password;
    X::string email;
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
