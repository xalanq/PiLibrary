// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadReturnBook.h>
#include <client/utils.h>

ThreadReturnBook::ThreadReturnBook(const xll &token, const xint &userid, const xint &bookid, QObject *parent) :
    ThreadNetwork(parent),
    token(token),
    userid(userid),
    bookid(bookid) {
}

void ThreadReturnBook::run() {
    xll token = this->token;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    pt.put("userid", this->userid);
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

    emit done(ec);
}