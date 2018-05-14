// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/threadgetnewbooklist.h>

ThreadGetNewBookList::ThreadGetNewBookList(const X::xll &token, const X::xint &number, QObject *parent) :
    token(token),
    number(number),
    ThreadNetwork(parent) {
}

void ThreadGetNewBookList::run() {
    xll token = this->token;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    pt.put("number", this->number);

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::GetNewBookList, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &) {
        ec = X::UnknownError;
        token = 0;
        pt = ptree();
    }

    if (ac != X::GetNewBookListFeedback) {
        ec = X::InvalidBook;
        token = 0;
        pt = ptree();
    }

    emit done(ec, pt);
}