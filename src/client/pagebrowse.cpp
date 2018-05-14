// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QVBoxLayout>

#include <core/book.h>
#include <client/pagebrowse.h>

GetBookThread::GetBookThread(const xll &token, const xint &bookid, QObject *parent) :
    token(token),
    bookid(bookid),
    NetworkThread(parent) {
}

void GetBookThread::run() {
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
        token = 0;
        pt = ptree();
    }

    if (ac != X::GetBookFeedback) {
        ec = X::InvalidBook;
        token = 0;
        pt = ptree();
    }

    emit done(ec, pt);
}


GetNewBookListThread::GetNewBookListThread(const X::xll &token, const X::xint &number, QObject *parent) :
    token(token),
    number(number),
    NetworkThread(parent) {
}

void GetNewBookListThread::run() {
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

PageBrowse::PageBrowse(UserManager &userManager, QWidget *parent) :
    userManager(userManager),
    QWidget(parent) {

    listWidgetBook = new ListWidgetBook(this);

    setUI();
    setConnection();
}

void PageBrowse::slotEndGetBook(const X::ErrorCode, const ptree &pt) {
    Book book;
    book.setTitle(pt.get("title", "no title"));
    listWidgetBook->addBook(book);
}

void PageBrowse::slotEndGetNewBookList(const X::ErrorCode &ec, const ptree &pt) {
    auto arr = pt.get_child("bookid");
    for (auto &&child : arr) {
        auto bookid = child.second.get_value<X::xint>();
        auto thread = new GetBookThread(userManager.getToken(), bookid, this);
        connect(thread, &GetBookThread::done, this, &PageBrowse::slotEndGetBook);
        connect(thread, &GetBookThread::finished, thread, &QObject::deleteLater);
        thread->start();
    }
}

void PageBrowse::setUI() {
    auto layout = new QVBoxLayout;
    layout->addWidget(listWidgetBook);
    setLayout(layout);
    
    auto thread = new GetNewBookListThread(userManager.getToken(), 15, this);
    connect(thread, &GetNewBookListThread::done, this, &PageBrowse::slotEndGetNewBookList);
    connect(thread, &GetNewBookListThread::finished, thread, &QObject::deleteLater);
    thread->start();
}

void PageBrowse::setConnection() {

}
