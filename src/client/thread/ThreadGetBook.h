// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <client/core/Resource.h>
#include <client/thread/ThreadNetwork.h>

class ThreadGetBook : public ThreadNetwork {
    Q_OBJECT

public:
    ThreadGetBook(const xll &token, const xint &bookid, bool brief, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec, const ptree &pt, const Resource &cover);

private:
    void run() override;

private:
    xll token;
    xint bookid;
    bool brief;
};