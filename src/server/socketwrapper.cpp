#include "socketwrapper.h"

SocketWrapper::SocketWrapper(boost::asio::ip::tcp::socket socket) :
    socket(std::move(socket)),
    info(),
    started(false) {

}

void SocketWrapper::start() {
    started = true;
    doLogin();
}

void SocketWrapper::stop() {
    if (!started)
        return;
    started = false;
    socket.close();
}

void SocketWrapper::doLogin() {
    auto self(shared_from_this());
}

bool SocketWrapper::isStarted() const {
    return started;
}
