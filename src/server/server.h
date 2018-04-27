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

    Server(const X::string &url, const int &port, const X::string &sql_url, const int &sql_port, const int &thread_number);
    static std::shared_ptr<Server> start(const X::string &url, const int &port, const X::string &sql_url, const int &sql_port, const int &thread_number);

private:
    void start();
    void doAccept();

    boost::asio::io_service service;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
    int thread_number;
    SessionManager sessionManager;
    UserManager userManager;
};

#endif // SERVER_H
