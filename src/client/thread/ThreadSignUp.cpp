// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadSignUp.h>
#include <client/utils.h>

ThreadSignUp::ThreadSignUp(const QString &username, const QString &nickname, const QString &password, const QString &email, QObject *parent) :
    ThreadNetwork(parent),
    username(username.toStdString()),
    nickname(nickname.toStdString()),
    password(password.toStdString()),
    email(email.toStdString()) {
}

void ThreadSignUp::run() {
    xll token = 0;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    pt.put("username", username);
    pt.put("nickname", nickname);
    pt.put("password", password); 
    pt.put("email", email); 

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::Register, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &) {
        ec = X::RegisterFailed;
    }

    emit done(ec);
}