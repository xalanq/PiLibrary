// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <ctime>
#include <functional>

#include <QVBoxLayout>

#include <client/dialog/DialogBook.h>
#include <client/page/PageFavorite.h>
#include <client/object/GetRecords.h>

PageFavorite::PageFavorite(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    QWidget(parent) {

    listWidgetStarRecord = new ListWidgetStarRecord(this);

    setUI();
    setConnection();
}

void PageFavorite::updateStar(const X::xint &bookid, bool star) {
    if (!star) {
        for (int i = 0; i < listWidgetStarRecord->count(); ++i) {
            auto item = dynamic_cast<ListWidgetItemStarRecord *> (listWidgetStarRecord->item(i));
            if (item->getBook().getBookid() == bookid) {
                listWidgetStarRecord->takeItem(i);
                break;
            }
        }
    } else {
        StarRecord record;
        record.setBookid(bookid);
        record.setTime(time(0));
        bookManager.getBookBrief(bookid, std::bind(&ListWidgetStarRecord::add, listWidgetStarRecord, std::placeholders::_1, record, 0));
    }
}

void PageFavorite::slotGetStarRecord(const std::vector<StarRecord> &records) {
    int tot = int(records.size());
    for (int i = 0; i < tot; ++i) {
        listWidgetStarRecord->add(BookBrief::unknown(), records[i]);
        userManager.starBook(records[i].getBookid());
    }
    for (int i = 0; i < tot; ++i)
        bookManager.getBookBrief(records[i].getBookid(), std::bind(&ListWidgetStarRecord::update, listWidgetStarRecord, std::placeholders::_1, records[i], tot - 1 - i));
    emit signalReady();
}

void PageFavorite::slotItemClicked(QListWidgetItem *item) {
    auto x = dynamic_cast<ListWidgetItemStarRecord *> (item);
    auto dialog = new DialogBook(userManager, bookManager, x->getBook().getBookid(), this);
    connect(dialog, SIGNAL(signalModify()), this, SIGNAL(signalModify()));
    dialog->show();
}

void PageFavorite::refresh() {
    userManager.clearStar();
    listWidgetStarRecord->clear();
    auto obj = new GetStarRecords(userManager.getToken(), bookManager, 2147483647, 0);
    connect(obj, &GetStarRecords::done, this, &PageFavorite::slotGetStarRecord);
    obj->start();
}

void PageFavorite::setUI() {
    auto layout = new QVBoxLayout;
    layout->addWidget(listWidgetStarRecord);
    setLayout(layout);
}

void PageFavorite::setConnection() {
    connect(listWidgetStarRecord,
            &ListWidgetStarRecord::itemDoubleClicked,
            this,
            &PageFavorite::slotItemClicked);
}

