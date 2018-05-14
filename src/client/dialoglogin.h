// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>

#include <client/networkthread.h>
#include <client/usermanager.h>
#include <client/xclient.h>

class LoginThread : public NetworkThread {
    Q_OBJECT

public:
    LoginThread(const QString &username, const QString &password, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec, const xll &token, const ptree &pt);

private:
    void run() override;

private:
    xstring username;
    xstring password;
};

class DialogLogin : public QDialog {
    Q_OBJECT

public:
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

