// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadSetPriority.h>
#include <client/utils.h>

ThreadSetPriority::ThreadSetPriority(const xll &token, const xint &userid, const xint &priority, QObject *parent) :
    token(token),
    userid(userid),
    priority(priority),
    ThreadNetwork(parent) {
}

void ThreadSetPriority::run() {
    xll token = this->token;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;
    
    pt.put("userid", this->userid);
    pt.put("priority", this->priority);

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::SetPriority, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &e) {
        auto str = e.what();
        ec = X::UnknownError;
    }

    emit done(ec);
}
