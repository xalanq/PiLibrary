// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QThread>
#include <QWidget>

#include <client/usermanager.h>
#include <client/xclient.h>

class GetBookThread : public QThread {
    Q_OBJECT

public:
    typedef boost::property_tree::ptree ptree;

    GetBookThread(const X::xll &token, const X::xint &bookid, QObject *parent = Q_NULLPTR);

signals:
    void done(const X::ErrorCode &ec, const X::xll &token, const ptree &pt);

private:
    void run() override;

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint ep;

    X::xll token;
    X::xint bookid;
};

class PageBrowse : public QWidget {
    Q_OBJECT

public:
    PageBrowse(UserManager &userManager, QWidget *parent = Q_NULLPTR);

private:
    UserManager &userManager;
};
