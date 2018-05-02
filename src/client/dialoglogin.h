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
#include <QPushButton>
#include <QVBoxLayout>

#include <client/xclient.h>

class DialogLogin : public QDialog {
    Q_OBJECT

public:
    DialogLogin(boost::asio::io_service &io_service, QWidget *parent = Q_NULLPTR);

signals:
    void signalLoginToken(const X::ull &token);

public slots:
    void slotLogin();

private:
    void setUI();
    void setConnection();
    void loadSetting();

private:
    boost::asio::io_service &io_service;

    QLabel *labelMessage;

    QComboBox *cbboxUsername;
    QLineEdit *editPassword;

    QDialogButtonBox *btns;
};

