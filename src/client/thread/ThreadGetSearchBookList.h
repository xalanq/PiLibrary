// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <client/thread/ThreadNetwork.h>

class ThreadGetSearchBookList : public ThreadNetwork {
    Q_OBJECT

public:
    ThreadGetSearchBookList(const xll &token, const ptree &pt, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec,  const ptree &pt);

private:
    void run() override;

private:
    xll token;
    ptree pt;
};