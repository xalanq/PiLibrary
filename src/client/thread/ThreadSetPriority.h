// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QString>

#include <client/thread/ThreadNetwork.h>

class ThreadSetPriority : public ThreadNetwork {
    Q_OBJECT

public:
    ThreadSetPriority(const xll &token, const xint &userid, const xint &priority, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec);

private:
    void run() override;
    
private:
    xll token;
    xint userid;
    xint priority;
};