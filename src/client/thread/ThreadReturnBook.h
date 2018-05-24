// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <client/thread/ThreadNetwork.h>

class ThreadReturnBook : public ThreadNetwork {
    Q_OBJECT

public:
    ThreadReturnBook(const xll &token, const xint &userid, const xint &bookid, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec);

private:
    void run() override;

private:
    xll token;
    xint userid;
    xint bookid;
};