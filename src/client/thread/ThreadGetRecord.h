// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <client/thread/ThreadNetwork.h>

class ThreadGetRecord : public ThreadNetwork {
    Q_OBJECT

public:
    ThreadGetRecord(const xll &token, const ActionCode &type, const xint &number, const xint& begin = 0, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec, const ptree &pt);

private:
    void run() override;

private:
    xll token;
    ActionCode type;
    xint number;
    xint begin;
};