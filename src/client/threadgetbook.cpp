// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/threadgetbook.h>

ThreadGetBook::ThreadGetBook(const xll &token, const xint &bookid, QObject *parent) :
    token(token),
    bookid(bookid),
    ThreadNetwork(parent) {
}

void ThreadGetBook::run() {
    xll token = this->token;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    pt.put("bookid", this->bookid);

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::GetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &) {
        ec = X::InvalidBook;
        pt = ptree();
    }

    if (ac != X::GetBookFeedback) {
        ec = X::InvalidBook;
        pt = ptree();
    }

    emit done(ec, pt);
}