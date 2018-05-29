// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <client/thread/ThreadNetwork.h>

class ThreadGetTopBookList : public ThreadNetwork {
    Q_OBJECT

public:
    ThreadGetTopBookList(const xll &token, const xint &number, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec,  const ptree &pt);

private:
    void run() override;

private:
    xll token;
    xint number;
};