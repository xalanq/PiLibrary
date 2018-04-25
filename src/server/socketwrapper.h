#ifndef SOCKETWRAPPER_H
#define SOCKETWRAPPER_H

#include "xserver.h"
#include "sessionmanager.h"
#include "socketinfo.h"
#include <boost/asio.hpp>
#include <ctime>

class SocketWrapper : public std::enable_shared_from_this<chat_session> {
public:
    using boost::asio;
    using boost::asio::ip;
    typedef boost::system::system_error system_error;
    typedef boost::system::error_code error_code;

    SocketWrapper(ip::tcp::socket socket, SessionManager &manager);
    void start();
    void stop();
    bool isStarted() const;

private:
    void doLogin();

    ip::tcp::socket socket;
    SocketInfo info;
    bool started;
};

#endif // SOCKETWRAPPER_H
