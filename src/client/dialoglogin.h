// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <boost/asio.hpp>

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QThread>

#include <client/xclient.h>

class LoginThread : public QThread {
    Q_OBJECT

public:
    LoginThread(const QString &username, const QString &password, QObject *parent = Q_NULLPTR);

signals:
    void done(const long long &token, const int &ec);

private:
    void run() override;

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint ep;
    X::xstring username;
    X::xstring password;
};

class DialogLogin : public QDialog {
    Q_OBJECT

public:
    DialogLogin(QWidget *parent = Q_NULLPTR);

signals:
    void done(const long long &token);

public slots:
    void slotLoginBegin();
    void slotLoginEnd(const X::xll &token, const int &ec);
    void slotRegister();

private:
    void setUI();
    void setConnection();
    void loadSetting();

private:
    QComboBox *cbboxUsername;
    QLineEdit *editPassword;

    QLabel *labelMessage;
    
    QPushButton *btnSignUp;
    QDialogButtonBox *btns;
};

