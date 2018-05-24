// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QString>

#include <client/thread/ThreadNetwork.h>

class ThreadLogout : public ThreadNetwork {
    Q_OBJECT

public:
    ThreadLogout(const xll &token, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec);

private:
    void run() override;
    
private:
    xll token;
};