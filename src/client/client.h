// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QtWidgets/QMainWindow>

#include <client/usermanager.h>
#include <client/xclient.h>

class Client : public QMainWindow {
    Q_OBJECT

public:
    typedef boost::property_tree::ptree ptree;
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;

    Client(QWidget *parent = Q_NULLPTR);

private:
    void setUI();
    void setConnection();

private:
    UserManager userManager;

};
