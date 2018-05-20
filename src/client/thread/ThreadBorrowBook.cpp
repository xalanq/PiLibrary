// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadBorrowBook.h>
#include <core/utils.h>

ThreadBorrowBook::ThreadBorrowBook(const xll &token, const xint &bookid, const xll &keepTime, QObject *parent) :
    token(token),
    bookid(bookid),
    keepTime(keepTime),
    ThreadNetwork(parent) {
}

void ThreadBorrowBook::run() {
    xll token = this->token;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    pt.put("bookid", this->bookid);
    pt.put("keepTime", this->keepTime);

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::BorrowBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &) {
        ec = X::UnknownError;
    }

    if (ac != X::BorrowBookFeedback) {
        ec = X::UnknownError;
    }

    emit done(ec);
}