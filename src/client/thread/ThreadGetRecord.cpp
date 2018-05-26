// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/thread/ThreadGetRecord.h>
#include <client/utils.h>

ThreadGetRecord::ThreadGetRecord(const xll &token, const ActionCode &type, const xint &number, const xint &begin, QObject *parent) :
    type(type),
    token(token),
    number(number),
    begin(begin),
    ThreadNetwork(parent) {
}

void ThreadGetRecord::run() {
    xll token = this->token;
    ptree pt;
    ActionCode ac = X::NoAction;
    ErrorCode ec = X::NoError;

    pt.put("number", this->number);
    pt.put("begin", this->begin);

    try {
        auto socket = newSocket();
        X::tcp_sync_write(socket, token, this->type, pt);
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