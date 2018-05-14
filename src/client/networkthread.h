// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <boost/asio.hpp>

#include <QThread>

#include <client/xclient.h>

class NetworkThread : public QThread {
    Q_OBJECT

public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;
    typedef X::ErrorCode ErrorCode;
    typedef X::ActionCode ActionCode;

    NetworkThread(QObject *parent = Q_NULLPTR);
    virtual ~NetworkThread() = default;

    boost::asio::ip::tcp::socket newSocket();

private:
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint ep;
};
