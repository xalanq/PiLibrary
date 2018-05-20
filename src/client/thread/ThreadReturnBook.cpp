// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadReturnBook.h>
#include <core/utils.h>

ThreadReturnBook::ThreadReturnBook(const xll &token, const xint &bookid, QObject *parent) :
    token(token),
    bookid(bookid),
    ThreadNetwork(parent) {
}

void ThreadReturnBook::run() {
    xll token = this->token;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    pt.put("bookid", this->bookid);

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::ReturnBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &) {
        ec = X::UnknownError;
    }

    if (ac != X::ReturnBookFeedback) {
        ec = X::UnknownError;
    }

    emit done(ec);
}