// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <vector>

#include <QObject>

#include <client/core/BrowseRecord.h>
#include <client/core/BorrowRecord.h>
#include <client/core/KeepRecord.h>
#include <client/core/StarRecord.h>
#include <client/manager/BookManager.h>
#include <core/types.h>

class GetBrowseRecords : public QObject {
    Q_OBJECT

public:
    GetBrowseRecords(const X::xll &token, BookManager &bookManager, const X::xint &number, const X::xint &begin, QObject *parent = Q_NULLPTR);
    void start();

signals:
    void done(const std::vector<BrowseRecord> &records);

public slots:
    void slotEnd(const X::ErrorCode &ec, const X::ptree &pt);

private:
    X::xll token;
    BookManager &bookManager;
    X::xint number;
    X::xint begin;
    std::vector<BrowseRecord> recordList;
};

class GetKeepRecords : public QObject {
    Q_OBJECT

public:
    GetKeepRecords(const X::xll &token, BookManager &bookManager, const X::xint &number, const X::xint &begin, QObject *parent = Q_NULLPTR);
    void start();

signals:
    void done(const std::vector<KeepRecord> &records);

public slots:
    void slotEnd(const X::ErrorCode &ec, const X::ptree &pt);

private:
    X::xll token;
    BookManager &bookManager;
    X::xint number;
    X::xint begin;
    std::vector<KeepRecord> recordList;
};

class GetBorrowRecords : public QObject {
    Q_OBJECT

public:
    GetBorrowRecords(const X::xll &token, BookManager &bookManager, const X::xint &number, const X::xint &begin, QObject *parent = Q_NULLPTR);
    void start();

signals:
    void done(const std::vector<BorrowRecord> &records);

public slots:
    void slotEnd(const X::ErrorCode &ec, const X::ptree &pt);

private:
    X::xll token;
    BookManager &bookManager;
    X::xint number;
    X::xint begin;
    std::vector<BorrowRecord> recordList;
};

class GetStarRecords : public QObject {
    Q_OBJECT

public:
    GetStarRecords(const X::xll &token, BookManager &bookManager, const X::xint &number, const X::xint &begin, QObject *parent = Q_NULLPTR);
    void start();

signals:
    void done(const std::vector<StarRecord> &records);

public slots:
    void slotEnd(const X::ErrorCode &ec, const X::ptree &pt);

private:
    X::xll token;
    BookManager &bookManager;
    X::xint number;
    X::xint begin;
    std::vector<StarRecord> recordList;
};
