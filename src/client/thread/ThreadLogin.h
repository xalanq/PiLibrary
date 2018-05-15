// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QString>

#include <client/thread/ThreadNetwork.h>

class ThreadLogin : public ThreadNetwork {
    Q_OBJECT

public:
    ThreadLogin(const QString &username, const QString &password, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec, const xll &token, const ptree &pt);

private:
    void run() override;

private:
    xstring username;
    xstring password;
};