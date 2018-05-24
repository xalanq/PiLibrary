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

#include <client/manager/UserManager.h>
#include <core/types.h>

class DialogLogin : public QDialog {
    Q_OBJECT

public:
    DialogLogin(UserManager &userManager, QWidget *parent = Q_NULLPTR);
    ~DialogLogin();
    void loadSetting();
    void saveSetting();

public slots:
    void slotLoginBegin();
    void slotLoginEnd(const X::ErrorCode &ec, const X::xll &token, const X::ptree &pt);
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
    QPushButton *btnLogin;
};

