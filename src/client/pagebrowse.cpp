// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/pagebrowse.h>

GetBookThread::GetBookThread(const X::xll &token, const X::xint &bookid, QObject *parent) :
    token(token),
    bookid(bookid),
    QThread(parent) {

    qRegisterMetaType<X::ErrorCode>("X::ErrorCode");
    qRegisterMetaType<X::xll>("X::xll");
    qRegisterMetaType<ptree>("ptree");
}

void GetBookThread::run() {
    X::xll token = this->token;
    ptree pt;
    X::ActionCode ac = X::NoAction;
    X::ErrorCode ec = X::NoError;

    pt.put("bookid", this->bookid);

    try {
        boost::asio::ip::tcp::socket socket(io_service);
        socket.connect(ep);
        X::tcp_sync_write(socket, token, X::GetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<X::ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &) {
        ec = X::InvalidBook;
        token = 0;
        pt = ptree();
    }

    if (ac != X::GetBookFeedback) {
        ec = X::InvalidBook;
        token = 0;
        pt = ptree();
    }

    emit done(ec, token, pt);
}

PageBrowse::PageBrowse(UserManager &userManager, QWidget *parent) :
    userManager(userManager),
    QWidget(parent) {
}
