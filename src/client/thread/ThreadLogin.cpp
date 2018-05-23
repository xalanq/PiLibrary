// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadLogin.h>
#include <core/utils.h>

ThreadLogin::ThreadLogin(const QString &username, const QString &password, QObject *parent) :
    username(username.toStdString()),
    password(password.toStdString()),
    ThreadNetwork(parent) {
}

void ThreadLogin::run() {
    xll token = 0;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    pt.put("username", username);
    pt.put("password", password); 

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::Login, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &e) {
        auto str = e.what();
        ec = X::LoginFailed;
        token = 0;
        pt = ptree();
    }

    emit done(ec, token, pt);
}
