// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <client/thread/ThreadNetwork.h>
#include <core/Resource.h>

class ThreadSetBook : public ThreadNetwork {
    Q_OBJECT

public:
    ThreadSetBook(const xll &token, const ptree &pt, const Resource &cover, QObject *parent = Q_NULLPTR);

signals:
    void done(const ErrorCode &ec);

private:
    void run() override;

private:
    xll token;
    ptree pt;
    Resource cover;
};
