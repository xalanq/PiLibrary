// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadGetTopBookList.h>
#include <client/utils.h>

ThreadGetTopBookList::ThreadGetTopBookList(const xll &token, const xint &number, QObject *parent) :
    token(token),
    number(number),
    ThreadNetwork(parent) {
}

void ThreadGetTopBookList::run() {
    xll token = this->token;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    pt.put("number", this->number);

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::GetTopBookList, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &) {
        ec = X::UnknownError;
        pt = ptree();
    }

    emit done(ec, pt);
}