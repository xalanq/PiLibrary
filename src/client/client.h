// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <boost/asio.hpp>

#include <QtWidgets/QMainWindow>

class Client : public QMainWindow {
    Q_OBJECT

public:
    Client(QWidget *parent = Q_NULLPTR);

private:
    boost::asio::io_service io_service;
};
