// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QVBoxLayout>

#include <client/getrecords.h>
#include <client/pagefavorite.h>

PageFavorite::PageFavorite(UserManager &userManager, BookManager &bookManager, QWidget *parent) :
    userManager(userManager),
    bookManager(bookManager),
    QWidget(parent) {

    listWidgetStarRecord = new ListWidgetStarRecord(this);

    setUI();
}

void PageFavorite::slotGetStarRecord(const std::vector<StarRecord> &records) {
    for (auto &&record : records)
        listWidgetStarRecord->add(bookManager.get(record.getBookid()), record);
}

void PageFavorite::setUI() {
    auto layout = new QVBoxLayout;
    layout->addWidget(listWidgetStarRecord);
    setLayout(layout);

    auto obj = new GetStarRecords(userManager.getToken(), bookManager, 15, 0);
    connect(obj, &GetStarRecords::done, this, &PageFavorite::slotGetStarRecord);
    obj->start();
}

