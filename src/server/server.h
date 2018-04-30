#ifndef SERVER_H
#define SERVER_H

#include "xserver.h"
#include "socketwrapper.h"
#include "sessionmanager.h"
#include "usermanager.h"
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

class Server {
public:
    typedef boost::system::system_error system_error;
    typedef boost::system::error_code error_code;

    Server(boost::asio::io_service &service, const X::string &url, const int &port, const X::string &mongo_url, const X::string &mongo_db_name, const X::uint &default_alive, const int &thread_number);
    static void start(const X::string &url, const int &port, const X::string &mongo_url, const X::string &mongo_db_name, const X::uint &default_alive, const int &thread_number);

private:
    void start();
    void doAccept();

    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
    int thread_number;
    UserManager userManager;
    SessionManager sessionManager;
};

#endif // SERVER_H
