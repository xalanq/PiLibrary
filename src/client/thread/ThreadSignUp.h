// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QString>

#include <client/thread/ThreadNetwork.h>

class ThreadSignUp : public ThreadNetwork {
    Q_OBJECT

public:
    ThreadSignUp(const QString &username, const QString &nickname, const QString &password, const QString &email, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec);

private:
    void run() override;

private:
    xstring username;
    xstring nickname;
    xstring password;
    xstring email;
};