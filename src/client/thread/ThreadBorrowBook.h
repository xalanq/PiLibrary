// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <client/thread/ThreadNetwork.h>

class ThreadBorrowBook : public ThreadNetwork {
    Q_OBJECT

public:
    ThreadBorrowBook(const xll &token, const xint &bookid, const xll &keepTime, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec);

private:
    void run() override;

private:
    xll token;
    xint bookid;
    xll keepTime;
};