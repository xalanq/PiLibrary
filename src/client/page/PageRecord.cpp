// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <memory>

#include <QVBoxLayout>

#include <client/page/PageRecord.h>
#include <client/thread/ThreadGetRecord.h>
#include <client/utils/GetRecords.h>

PageRecord::PageRecord(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    QWidget(parent) {

    tabWidget = new QTabWidget(this);
    listWidgetBrowseRecord = new ListWidgetBrowseRecord(this);
    listWidgetKeepRecord = new ListWidgetKeepRecord(this);
    listWidgetBorrowRecord = new ListWidgetBorrowRecord(this);
    listWidgetLoginRecord = new ListWidgetLoginRecord(this);

    setUI();
}

void PageRecord::slotGetBrowseRecord(const std::vector<BrowseRecord> &records) {
    for (auto &&record : records)
        listWidgetBrowseRecord->add(bookManager.getBookBrief(record.getBookid()), record);
}

void PageRecord::slotGetKeepRecord(const std::vector<KeepRecord> &records) {
    for (auto &&record : records)
        listWidgetKeepRecord->add(bookManager.getBookBrief(record.getBookid()), record);
}

void PageRecord::slotGetBorrowRecord(const std::vector<BorrowRecord> &records) {
    for (auto &&record : records)
        listWidgetBorrowRecord->add(bookManager.getBookBrief(record.getBookid()), record);
}

void PageRecord::slotGetLoginRecord(const X::ErrorCode &ec, const X::ptree &pt) {
    if (ec == X::NoError) {
        auto arr = pt.get_child("loginRecord");
        arr.reverse();
        for (auto &&child : arr) {
            auto ip = child.second.get<X::xstring>("ip", "");
            auto time = child.second.get<X::xll>("time", 0);
            LoginRecord record;
            record.setIp(ip);
            record.setTime(time);
            listWidgetLoginRecord->add(std::move(record));
        }
    }
}

void PageRecord::setUI() {
    tabWidget->addTab(listWidgetBrowseRecord, tr("Brows&e Record"));
    tabWidget->addTab(listWidgetKeepRecord, tr("&Keep Record"));
    tabWidget->addTab(listWidgetBorrowRecord, tr("Borro&w Record"));
    tabWidget->addTab(listWidgetLoginRecord, tr("&Login Record"));

    auto layout = new QVBoxLayout;
    layout->addWidget(tabWidget);

    setLayout(layout);

    auto obj1 = new GetBrowseRecords(userManager.getToken(), bookManager, 15, 0);
    connect(obj1, &GetBrowseRecords::done, this, &PageRecord::slotGetBrowseRecord);
    obj1->start();

    auto obj2 = new GetKeepRecords(userManager.getToken(), bookManager, 15, 0);
    connect(obj2, &GetKeepRecords::done, this, &PageRecord::slotGetKeepRecord);
    obj2->start();

    auto obj3 = new GetBorrowRecords(userManager.getToken(), bookManager, 15, 0);
    connect(obj3, &GetBorrowRecords::done, this, &PageRecord::slotGetBorrowRecord);
    obj3->start();

    auto thread= new ThreadGetRecord(userManager.getToken(), X::GetLoginRecord, 15, 0, this);
    connect(thread, &ThreadGetRecord::done, this, &PageRecord::slotGetLoginRecord);
    connect(thread, &ThreadGetRecord::finished, thread, &QObject::deleteLater);
    thread->start();
}
