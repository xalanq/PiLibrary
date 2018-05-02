// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <boost/asio.hpp>

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QThread>
#include <QVBoxLayout>

#include <client/xclient.h>

class LoginThread : public QThread {
    Q_OBJECT

public:
    LoginThread(const QString &username, const QString &password, QObject *parent = Q_NULLPTR);

signals:
    void done(const unsigned long long &token, const int &ec);

private:
    void run() override;

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint ep;
    QString username;
    QString password;
};

class DialogLogin : public QDialog {
    Q_OBJECT

public:
    DialogLogin(QWidget *parent = Q_NULLPTR);

signals:
    void done(const X::ull &token);

public slots:
    void slotLoginBegin();
    void slotLoginEnd(const unsigned long long &token, const int &ec);

private:
    void setUI();
    void setConnection();
    void loadSetting();

private:
    QLabel *labelMessage;

    QComboBox *cbboxUsername;
    QLineEdit *editPassword;

    QDialogButtonBox *btns;
};

