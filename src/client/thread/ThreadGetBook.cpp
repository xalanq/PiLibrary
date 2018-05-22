// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadGetBook.h>
#include <core/utils.h>

ThreadGetBook::ThreadGetBook(const xll &token, const xint &bookid, bool brief, QObject *parent) :
    token(token),
    bookid(bookid),
    brief(brief),
    ThreadNetwork(parent) {
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
            char *buffer = nullptr;
            size_t size = 0;
            buffer = X::tcp_sync_read_with_file(socket, token, a, p, size);
            if (size && a == X::GetBookCoverFeedback)
                cover.setData(buffer)
                     .setSize(size);
        }
        socket.close();
    } catch (std::exception &e) {
        auto s = e.what();
        ec = X::InvalidBook;
        pt = ptree();
        cover.clean();
        cover = Resource();
    }

    if (ac != (brief ? X::GetBookBriefFeedback : X::GetBookFeedback)) {
        ec = X::InvalidBook;
        pt = ptree();
        cover.clean();
        cover = Resource();
    }

    emit done(ec, pt, cover);
}