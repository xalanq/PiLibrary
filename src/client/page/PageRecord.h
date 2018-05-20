// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <vector>

#include <QTabWidget>
#include <QWidget>

#include <client/listwidget/ListWidgetBorrowRecord.h>
#include <client/listwidget/ListWidgetBrowseRecord.h>
#include <client/listwidget/ListWidgetKeepRecord.h>
#include <client/listwidget/ListWidgetLoginRecord.h>
#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>
#include <core/types.h>

class PageRecord : public QWidget {
    Q_OBJECT

public:
    PageRecord(UserManager &userManager, BookManager &bookManager, QWidget *parent = Q_NULLPTR);

public slots:
    void slotGetBrowseRecord(const std::vector<BrowseRecord> &records);

    void slotGetKeepRecord(const std::vector<KeepRecord> &records);

    void slotGetBorrowRecord(const std::vector<BorrowRecord> &records);

    void slotGetLoginRecord(const X::ErrorCode &ec, const X::ptree &pt);

    void slotBrowseRecordItemClicked(QListWidgetItem *item);

    void slotKeepRecordItemClicked(QListWidgetItem *item);

    void slotBorrowRecordItemClicked(QListWidgetItem *item);

private:
    void setUI();

    void setConnection();

private:
    UserManager &userManager;
    BookManager &bookManager;

    QTabWidget *tabWidget;
    ListWidgetBrowseRecord *listWidgetBrowseRecord;
    ListWidgetKeepRecord *listWidgetKeepRecord;
    ListWidgetBorrowRecord *listWidgetBorrowRecord;
    ListWidgetLoginRecord *listWidgetLoginRecord;
};
