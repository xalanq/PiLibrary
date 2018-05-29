// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadLogout.h>
#include <client/utils.h>

ThreadLogout::ThreadLogout(const xll &token, QObject *parent) :
    ThreadNetwork(parent),
    token(token) {
}

void ThreadLogout::run() {
    xll token = this->token;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::Logout, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &) {
        ec = X::UnknownError;
    }

    emit done(ec);
}
