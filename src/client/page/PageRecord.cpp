// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <ctime>
#include <memory>

#include <QVBoxLayout>

#include <client/dialog/DialogBook.h>
#include <client/page/PageRecord.h>
#include <client/thread/ThreadGetRecord.h>
#include <client/object/GetRecords.h>

PageRecord::PageRecord(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    QWidget(parent),
    userManager(userManager),
    bookManager(bookManager) {

    tabWidget = new QTabWidget(this);
    listWidgetBrowseRecord = new ListWidgetBrowseRecord(this);
    listWidgetKeepRecord = new ListWidgetKeepRecord(this);
    listWidgetBorrowRecord = new ListWidgetBorrowRecord(this);
    listWidgetLoginRecord = new ListWidgetLoginRecord(this);

    setUI();
    setConnection();
}

void PageRecord::updateBorrow(const X::xint &bookid, const X::xll &beginTime, const X::xll &endTime) {
    KeepRecord record1;
    record1.setBookid(bookid);
    record1.setTime(beginTime, endTime);
    bookManager.getBookBrief(bookid, std::bind(&ListWidgetKeepRecord::add, listWidgetKeepRecord, std::placeholders::_1, record1, 0));

    BorrowRecord record2;
    record2.setBeginTime(bookid);
    record2.setTime(beginTime, endTime, 0);
    bookManager.getBookBrief(bookid, std::bind(&ListWidgetBorrowRecord::add, listWidgetBorrowRecord, std::placeholders::_1, record2, 0));
}

void PageRecord::updateBrowse(const X::xint &bookid) {
    BrowseRecord record;
    record.setBookid(bookid);
    record.setTime(time(0));
    bookManager.getBookBrief(bookid, std::bind(&ListWidgetBrowseRecord::add, listWidgetBrowseRecord, std::placeholders::_1, record, 0));
}

void PageRecord::slotGetBrowseRecord(const std::vector<BrowseRecord> &records) {
    int tot = int(records.size());
    for (int i = 0; i < tot; ++i)
        listWidgetBrowseRecord->add(BookBrief::unknown(), records[i]);
    for (int i = 0; i < tot; ++i)
        bookManager.getBookBrief(records[i].getBookid(), std::bind(&ListWidgetBrowseRecord::update, listWidgetBrowseRecord, std::placeholders::_1, records[i], tot - 1 - i));
}

void PageRecord::slotGetKeepRecord(const std::vector<KeepRecord> &records) {
    int tot = int(records.size());
    for (int i = 0; i < tot; ++i) {
        listWidgetKeepRecord->add(BookBrief::unknown(), records[i]);
        userManager.borrowBook(records[i].getBookid(), records[i].getBeginTime(), records[i].getEndTime());
    }
    for (int i = 0; i < tot; ++i)
        bookManager.getBookBrief(records[i].getBookid(), std::bind(&ListWidgetKeepRecord::update, listWidgetKeepRecord, std::placeholders::_1, records[i], tot - 1 - i));
    emit signalReady();
}

void PageRecord::slotGetBorrowRecord(const std::vector<BorrowRecord> &records) {
    int tot = int(records.size());
    for (int i = 0; i < tot; ++i)
        listWidgetBorrowRecord->add(BookBrief::unknown(), records[i]);
    for (int i = 0; i < tot; ++i)
        bookManager.getBookBrief(records[i].getBookid(), std::bind(&ListWidgetBorrowRecord::update, listWidgetBorrowRecord, std::placeholders::_1, records[i], tot - 1 - i));
}

void PageRecord::slotGetLoginRecord(const X::ErrorCode &ec, const X::ptree &pt) {
    if (ec != X::NoError)
        return;
    auto arr = pt.get_child("loginRecord");
    arr.reverse();
    for (auto &&child : arr)
        listWidgetLoginRecord->add(LoginRecord::fromPtree(child.second));
}

void PageRecord::slotBrowseRecordItemClicked(QListWidgetItem *item) {
    auto x = dynamic_cast<ListWidgetItemBrowseRecord *> (item);
    auto dialog = new DialogBook(userManager, bookManager, x->getBook().getBookid(), this);
    connect(dialog, SIGNAL(signalModify()), this, SIGNAL(signalModify()));
    dialog->show();
}

void PageRecord::slotKeepRecordItemClicked(QListWidgetItem *item) {
    auto x = dynamic_cast<ListWidgetItemKeepRecord *> (item);
    auto dialog = new DialogBook(userManager, bookManager, x->getBook().getBookid(), this);
    connect(dialog, SIGNAL(signalModify()), this, SIGNAL(signalModify()));
    dialog->show();
}

void PageRecord::slotBorrowRecordItemClicked(QListWidgetItem *item) {
    auto x = dynamic_cast<ListWidgetItemBorrowRecord *> (item);
    auto dialog = new DialogBook(userManager, bookManager, x->getBook().getBookid(), this);
    connect(dialog, SIGNAL(signalModify()), this, SIGNAL(signalModify()));
    dialog->show();
}

void PageRecord::refresh() {
    refreshBrowseRecord();
    refreshKeepRecord();
    refreshBorrowRecord();
    refreshLoginRecord();
}

void PageRecord::refreshBrowseRecord() {
    listWidgetBrowseRecord->clear();
    auto obj = new GetBrowseRecords(userManager.getToken(), bookManager, 15, 0);
    connect(obj, &GetBrowseRecords::done, this, &PageRecord::slotGetBrowseRecord);
    obj->start();
}

void PageRecord::refreshKeepRecord() {
    userManager.clearBorrowBook();
    listWidgetKeepRecord->clear();
    auto obj = new GetKeepRecords(userManager.getToken(), bookManager, 2147483647, 0);
    connect(obj, &GetKeepRecords::done, this, &PageRecord::slotGetKeepRecord);
    obj->start();
}

void PageRecord::refreshBorrowRecord() {
    listWidgetBorrowRecord->clear();
    auto obj = new GetBorrowRecords(userManager.getToken(), bookManager, 15, 0);
    connect(obj, &GetBorrowRecords::done, this, &PageRecord::slotGetBorrowRecord);
    obj->start();
}

void PageRecord::refreshLoginRecord() {
    listWidgetLoginRecord->clear();
    auto thread= new ThreadGetRecord(userManager.getToken(), X::GetLoginRecord, 15, 0, this);
    connect(thread, &ThreadGetRecord::done, this, &PageRecord::slotGetLoginRecord);
    connect(thread, &ThreadGetRecord::finished, thread, &QObject::deleteLater);
    thread->start();
}

void PageRecord::setUI() {
    listWidgetBrowseRecord->setFrameStyle(QFrame::NoFrame);
    listWidgetKeepRecord->setFrameStyle(QFrame::NoFrame);
    listWidgetBorrowRecord->setFrameStyle(QFrame::NoFrame);
    listWidgetLoginRecord->setFrameStyle(QFrame::NoFrame);

    tabWidget->addTab(listWidgetBrowseRecord, tr("Brows&e Record"));
    tabWidget->addTab(listWidgetKeepRecord, tr("&Keep Record"));
    tabWidget->addTab(listWidgetBorrowRecord, tr("Borro&w Record"));
    tabWidget->addTab(listWidgetLoginRecord, tr("&Login Record"));

    auto layout = new QVBoxLayout;
    layout->addWidget(tabWidget);

    setLayout(layout);

    layout->setContentsMargins(0, 10, 0, 0);
}

void PageRecord::setConnection() {
    connect(listWidgetBrowseRecord,
            &ListWidgetBrowseRecord::itemDoubleClicked,
            this,
            &PageRecord::slotBrowseRecordItemClicked);
    connect(listWidgetKeepRecord,
            &ListWidgetKeepRecord::itemDoubleClicked,
            this,
            &PageRecord::slotKeepRecordItemClicked);
    connect(listWidgetBorrowRecord,
            &ListWidgetBorrowRecord::itemDoubleClicked,
            this,
            &PageRecord::slotBorrowRecordItemClicked);
}
