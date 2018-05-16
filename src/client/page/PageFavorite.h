// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/listwidget/ListWidgetStarRecord.h>
#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>

class PageFavorite : public QWidget {
    Q_OBJECT

public:
    PageFavorite(UserManager &userManager, BookManager &bookManager, QWidget *parent = Q_NULLPTR);

public slots:
    void slotGetStarRecord(const std::vector<StarRecord> &records);

private:
    void setUI();

private:
    UserManager &userManager;
    BookManager &bookManager;

    ListWidgetStarRecord *listWidgetStarRecord;
};
