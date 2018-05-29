// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadGetBook.h>
#include <client/utils.h>

ThreadGetBook::ThreadGetBook(const xll &token, const xint &bookid, bool brief, QObject *parent) :
    ThreadNetwork(parent),
    token(token),
    bookid(bookid),
    brief(brief) {
        
    qRegisterMetaType<Resource>("Resource");
}

void ThreadGetBook::run() {
    xll token = this->token;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;
    Resource cover;

    pt.put("bookid", this->bookid);

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, brief ? X::GetBookBrief : X::GetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
        if (ec == X::NoError) {
            ptree p;
            p.put("bookid", this->bookid);
            ActionCode a;
            X::tcp_sync_write(socket, token, X::GetBookCover, p);
            p = ptree();
            cover = X::tcp_sync_read_with_file(socket, token, a, p);
        }
        socket.close();
    } catch (std::exception &) {
        ec = X::InvalidBook;
        pt = ptree();
        cover.clean();
    }

    emit done(ec, pt, cover);
}