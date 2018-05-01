// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <memory>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <server/sessionmanager.h>
#include <server/socketwrapper.h>
#include <server/usermanager.h>
#include <server/xserver.h>

class Server {
public:
    typedef boost::system::system_error system_error;
    typedef boost::system::error_code error_code;
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;

    Server(boost::asio::io_service &service, const string &url, const int &port, const string &mongo_url, const string &mongo_db_name, const uint &default_alive, const int &thread_number);
    static void start(const string &url, const int &port, const string &mongo_url, const string &mongo_db_name, const uint &default_alive, const int &thread_number);

private:
    void start();
    void doAccept();

    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
    int thread_number;
    UserManager userManager;
    SessionManager sessionManager;
};
