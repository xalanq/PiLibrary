// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <memory>

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

#include <server/SessionManager.h>
#include <server/SocketWrapper.h>
#include <server/UserManager.h>
#include <core/types.h>

class Server {
public:
    typedef boost::system::system_error system_error;
    typedef boost::system::error_code error_code;
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;

    Server(boost::asio::io_service &service, const ptree &config);
    static void start(const ptree &config);

private:
    void start();
    void doAccept();
    void initRoot();

    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
    xint thread_number;
    UserManager userManager;
    SessionManager sessionManager;
};
