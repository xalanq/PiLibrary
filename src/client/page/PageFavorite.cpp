// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <functional>

#include <QVBoxLayout>

#include <client/dialog/DialogBook.h>
#include <client/page/PageFavorite.h>
#include <client/utils/GetRecords.h>

PageFavorite::PageFavorite(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    QWidget(parent) {

    listWidgetStarRecord = new ListWidgetStarRecord(this);

    setUI();
    setConnection();
}

void PageFavorite::slotGetStarRecord(const std::vector<StarRecord> &records) {
    int tot = records.size();
    for (int i = 0; i < tot; ++i)
        listWidgetStarRecord->add(BookBrief::unknown(), records[i]);
    for (int i = 0; i < tot; ++i)
        bookManager.getBookBrief(records[i].getBookid(), std::bind(&ListWidgetStarRecord::update, listWidgetStarRecord, std::placeholders::_1, records[i], tot - 1 - i));
}

void PageFavorite::slotItemClicked(QListWidgetItem *item) {
    auto x = dynamic_cast<ListWidgetItemStarRecord *> (item);
    DialogBook dialog(userManager, bookManager, x->getBook().getBookid());
    dialog.exec();
}

void PageFavorite::setUI() {
    auto layout = new QVBoxLayout;
    layout->addWidget(listWidgetStarRecord);
    setLayout(layout);

    auto obj = new GetStarRecords(userManager.getToken(), bookManager, 15, 0);
    connect(obj, &GetStarRecords::done, this, &PageFavorite::slotGetStarRecord);
    obj->start();
}

void PageFavorite::setConnection() {
    connect(listWidgetStarRecord,
            &ListWidgetStarRecord::itemDoubleClicked,
            this,
            &PageFavorite::slotItemClicked);
}

