// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QString>

#include <client/thread/ThreadNetwork.h>

class ThreadModifyUser : public ThreadNetwork {
    Q_OBJECT

public:
    ThreadModifyUser(const xll &token, const QString &nickname, const QString &email, const QString &passwordOld, const QString &passwordNew, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec);

private:
    void run() override;

private:
    xll token;
    xstring nickname;
    xstring email;
    xstring passwordOld;
    xstring passwordNew;
};