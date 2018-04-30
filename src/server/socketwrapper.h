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

// one read and one write as a turn
class SocketWrapper : public std::enable_shared_from_this<SocketWrapper> {
public:
    typedef boost::system::system_error system_error;
    typedef boost::system::error_code error_code;
    typedef boost::property_tree::ptree ptree;
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;
    typedef X::ErrorCode ErrorCode;
    typedef X::ActionCode ActionCode;

    SocketWrapper(boost::asio::ip::tcp::socket socket, SessionManager &sessionManager, UserManager &userManager);
    void start();
    void stop();

private:
    void read();
    void readHeader();
    void readBody(ull token, uint length, ActionCode ac);

    void write(const ull &token, const ptree &pt, const ActionCode &ac);

    void doLogin(const ptree &pt, const ull &token);
    void writeLogin(const ull &token, ptree pt, ErrorCode ec = X::NoError);

    void doRegister(const ptree &pt, const ull &token);
    void writeRegister(ErrorCode ec = X::NoError);

    void doLogout(const ptree &pt, const ull &token);
    void writeLogout(ErrorCode ec = X::NoError);

    boost::asio::ip::tcp::socket socket;
    SocketInfo info;
    SessionManager &sessionManager;
    UserManager &userManager;
};

#endif // SOCKETWRAPPER_H
