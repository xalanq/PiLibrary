#ifndef SERVER_H
#define SERVER_H

#include "xserver.h"
#include "socketwrapper.h"
#include "sessionmanager.h"
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

class Server {
public:
    using boost::asio::ip;
    typedef boost::system::system_error system_error;
    typedef boost::system::error_code error_code;

    Server(const X::string &url, const int &port, const X::string &sql_url, const int &sql_port, const int &thread_number);
    static std::shared_ptr<Server> start(const X::string &url, const int &port, const X::string &sql_url, const int &sql_port);

private:
    void start();
    void doAccept();

    boost::asio::io_service service;
    ip::tcp::acceptor acceptor;
    ip::tcp::socket socket;
    int thread_number;
    SessionManager manager;
};

#endif // SERVER_H
