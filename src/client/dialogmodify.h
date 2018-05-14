// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <client/networkthread.h>
#include <client/usermanager.h>
#include <client/xclient.h>

class ModifyThread : public NetworkThread {
    Q_OBJECT

public:
    ModifyThread(const xll &token, const QString &nickname, const QString &email, const QString &passwordOld, const QString &passwordNew, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec);

private:
    void run() override;

    xll token;
    xstring nickname;
    xstring email;
    xstring passwordOld;
    xstring passwordNew;
};

class DialogModify : public QDialog {
    Q_OBJECT

public:
    DialogModify(UserManager &userManager, QWidget *parent);

public slots:
    void slotModifyBegin();
    void slotModifyEnd(const int &ec);

private:
    void setUI();
    void setConnection();

    UserManager &userManager;

    QLineEdit *editNickname;
    QLineEdit *editEmail;
    QLineEdit *editPasswordOld;
    QLineEdit *editPasswordNew;
    QLineEdit *editPasswordNewConfirm;

    QLabel *labelMessage;

    QDialogButtonBox *btns;
};
