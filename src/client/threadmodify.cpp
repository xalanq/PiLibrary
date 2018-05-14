// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/threadmodify.h>

ThreadModify::ThreadModify(const xll &token, const QString &nickname, const QString &email, const QString &passwordOld, const QString &passwordNew, QObject *parent) :
    token(token),
    nickname(nickname.toStdString()),
    email(email.toStdString()),
    passwordOld(passwordOld.toStdString()),
    passwordNew(passwordNew.toStdString()), 
    ThreadNetwork(parent) {

}

void ThreadModify::run() {
    xll token = this->token;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    pt.put("nickname", nickname);
    pt.put("email", email); 
    pt.put("passwordOld", passwordOld);
    if (passwordNew.size() > 0)
        pt.put("passwordNew", passwordNew);

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, X::Modify, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        socket.close();
        ec = static_cast<ErrorCode> (pt.get<int>("error_code"));
    } catch (std::exception &) {
        ec = X::ModifyFailed;
    }

    if (ac != X::ModifyFeedback)
        ec = X::ModifyFailed;

    emit done(ec);
}