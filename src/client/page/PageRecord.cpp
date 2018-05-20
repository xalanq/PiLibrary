// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <memory>

#include <QVBoxLayout>

#include <client/dialog/DialogBook.h>
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
    setConnection();
}

void PageRecord::slotGetBrowseRecord(const std::vector<BrowseRecord> &records) {
    int tot = records.size();
    for (int i = 0; i < tot; ++i)
        listWidgetBrowseRecord->add(BookBrief::unknown(), records[i]);
    for (int i = 0; i < tot; ++i)
        bookManager.getBookBrief(records[i].getBookid(), std::bind(&ListWidgetBrowseRecord::update, listWidgetBrowseRecord, std::placeholders::_1, records[i], tot - 1 - i));
}

void PageRecord::slotGetKeepRecord(const std::vector<KeepRecord> &records) {
    int tot = records.size();
    for (int i = 0; i < tot; ++i) {
        listWidgetKeepRecord->add(BookBrief::unknown(), records[i]);
        userManager.getKeepBooks().insert(records[i].getBookid());
    }
    for (int i = 0; i < tot; ++i)
        bookManager.getBookBrief(records[i].getBookid(), std::bind(&ListWidgetKeepRecord::update, listWidgetKeepRecord, std::placeholders::_1, records[i], tot - 1 - i));
}

void PageRecord::slotGetBorrowRecord(const std::vector<BorrowRecord> &records) {
    int tot = records.size();
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
    DialogBook dialog(userManager, bookManager, x->getBook().getBookid());
    dialog.exec();
}

void PageRecord::slotKeepRecordItemClicked(QListWidgetItem *item) {
    auto x = dynamic_cast<ListWidgetItemKeepRecord *> (item);
    DialogBook dialog(userManager, bookManager, x->getBook().getBookid());
    dialog.exec();
}

void PageRecord::slotBorrowRecordItemClicked(QListWidgetItem *item) {
    auto x = dynamic_cast<ListWidgetItemBorrowRecord *> (item);
    DialogBook dialog(userManager, bookManager, x->getBook().getBookid());
    dialog.exec();
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

    auto obj2 = new GetKeepRecords(userManager.getToken(), bookManager, 2147483647, 0);
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
