// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QVBoxLayout>

#include <client/page/PageFavorite.h>
#include <client/utils/GetRecords.h>

PageFavorite::PageFavorite(UserManager &userManager, BookBriefManager &bookBriefManager, QWidget *parent) :
    userManager(userManager),
    bookBriefManager(bookBriefManager),
    QWidget(parent) {

    listWidgetStarRecord = new ListWidgetStarRecord(this);

    setUI();
}

void PageFavorite::slotGetStarRecord(const std::vector<StarRecord> &records) {
    for (auto &&record : records)
        listWidgetStarRecord->add(bookBriefManager.get(record.getBookid()), record);
}

void PageFavorite::setUI() {
    auto layout = new QVBoxLayout;
    layout->addWidget(listWidgetStarRecord);
    setLayout(layout);

    auto obj = new GetStarRecords(userManager.getToken(), bookBriefManager, 15, 0);
    connect(obj, &GetStarRecords::done, this, &PageFavorite::slotGetStarRecord);
    obj->start();
}

