#ifndef SOCKETWRAPPER_H
#define SOCKETWRAPPER_H

#include "xserver.h"
#include "../core/socketinfo.h"
#include "sessionmanager.h"
#include "usermanager.h"
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <ctime>
#include <exception>

class SocketWrapper : public std::enable_shared_from_this<SocketWrapper> {
public:
    typedef boost::system::system_error system_error;
    typedef boost::system::error_code error_code;

    SocketWrapper(boost::asio::ip::tcp::socket socket, SessionManager &sessionManager, UserManager &userManager);
    void start();
    void stop();

private:
    void doFilter();
    void doHeader();
    void doBody(X::ull token, X::uint length);
    void doMain(boost::property_tree::ptree pt, X::ull token, X::uint userid);

    boost::asio::ip::tcp::socket socket;
    SocketInfo info;
    SessionManager &sessionManager;
    UserManager &userManager;
};

#endif // SOCKETWRAPPER_H
