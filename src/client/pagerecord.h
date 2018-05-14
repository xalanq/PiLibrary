// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <vector>

#include <QTabWidget>
#include <QWidget>

#include <client/bookmanager.h>
#include <client/listwidgetborrowrecord.h>
#include <client/listwidgetbrowserecord.h>
#include <client/listwidgetkeeprecord.h>
#include <client/listwidgetloginrecord.h>
#include <client/usermanager.h>
#include <client/xclient.h>

class PageRecord : public QWidget {
    Q_OBJECT

public:
    PageRecord(UserManager &userManager, BookManager &bookManager, QWidget *parent = Q_NULLPTR);

public slots:
    void slotGetBrowseRecord(const std::vector<BrowseRecord> &records);
    void slotGetKeepRecord(const std::vector<KeepRecord> &records);
    void slotGetBorrowRecord(const std::vector<BorrowRecord> &records);
    void slotGetLoginRecord(const X::ErrorCode &ec, const ptree &pt);

private:
    void setUI();

private:
    UserManager &userManager;
    BookManager &bookManager;

    QTabWidget *tabWidget;
    ListWidgetBrowseRecord *listWidgetBrowseRecord;
    ListWidgetKeepRecord *listWidgetKeepRecord;
    ListWidgetBorrowRecord *listWidgetBorrowRecord;
    ListWidgetLoginRecord *listWidgetLoginRecord;
};
