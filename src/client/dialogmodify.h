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

#include <client/usermanager.h>
#include <client/xclient.h>

class ModifyThread : public QThread {
    Q_OBJECT

public:
    ModifyThread(const X::xll &token, const QString &nickname, const QString &email, const QString &passwordOld, const QString &passwordNew, QObject *parent = Q_NULLPTR);

signals:
    void done(const int &ec);

private:
    void run() override;

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint ep;
    X::xll token;
    X::xstring nickname;
    X::xstring email;
    X::xstring passwordOld;
    X::xstring passwordNew;
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
