// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadSetBook.h>
#include <client/utils.h>

ThreadSetBook::ThreadSetBook(const xll &token, const ptree &pt, const Resource &cover, QObject *parent) :
    token(token),
    pt(pt),
    cover(cover),
    ThreadNetwork(parent) {
}

void ThreadSetBook::run() {
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    auto bookid = pt.get<xint>("bookid");
    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));

        if (ec == X::NoError && cover.getSize()) {
            pt = ptree();
            pt.put("bookid", bookid);
            pt.put("fileSize", cover.getSize());
            X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
            pt = ptree();
            X::tcp_sync_read(socket, token, ac, pt);
            ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
        }
        socket.close();
    } catch (std::exception &) {
        ec = X::UnknownError;
    }

    emit done(ec);
}
