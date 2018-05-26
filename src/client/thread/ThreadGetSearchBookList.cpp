// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadGetSearchBookList.h>
#include <core/utils.h>

ThreadGetSearchBookList::ThreadGetSearchBookList(const xll &token, const ptree &pt, QObject *parent) :
    token(token),
    pt(pt),
    ThreadNetwork(parent) {
}

void ThreadGetSearchBookList::run() {
    xll token = this->token;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::GetSearchBookList, pt);
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