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

#include <client/usermanager.h>
#include <client/xclient.h>

class LoginThread : public QThread {
    Q_OBJECT

public:
    typedef boost::property_tree::ptree ptree;

    LoginThread(const QString &username, const QString &password, QObject *parent = Q_NULLPTR);

signals:
    void done(const X::ErrorCode &ec, const X::xll &token, const ptree &pt);

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
    typedef boost::property_tree::ptree ptree;

    DialogLogin(UserManager &userManager, QWidget *parent = Q_NULLPTR);
    ~DialogLogin();
    void loadSetting();
    void saveSetting();

public slots:
    void slotLoginBegin();
    void slotLoginEnd(const X::ErrorCode &ec, const X::xll &token, const ptree &pt);
    void slotRegister();

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;

    QComboBox *cbboxUsername;
    QLineEdit *editPassword;

    QLabel *labelMessage;
    
    QPushButton *btnSignUp;
    QDialogButtonBox *btns;
};

