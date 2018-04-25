#include "socketwrapper.h"

SocketWrapper::SocketWrapper(ip::tcp::socket socket) :
    socket(socket),
    info(),
    started(false) {

}

void SocketWrapper::start() {
    manager.add(shared_from_this());
    info.setLastPing(time());
    started = true;
    doLogin();
}

void SocketWrapper::stop() {
    if (!started)
        return;
    started = false;
    socket.close();
    manager.remove(shared_from_this());
}

void SocketWrapper::doLogin() {
    auto self(shared_from_this());
}

bool SocketWrapper::isStarted() const {
    return started;
}
