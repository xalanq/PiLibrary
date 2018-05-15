// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/listwidgetstarrecord.h>
#include <client/bookbriefmanager.h>
#include <client/usermanager.h>

class PageFavorite : public QWidget {
    Q_OBJECT

public:
    PageFavorite(UserManager &userManager, BookBriefManager &bookBriefManager, QWidget *parent = Q_NULLPTR);

public slots:
    void slotGetStarRecord(const std::vector<StarRecord> &records);

private:
    void setUI();

private:
    UserManager &userManager;
    BookBriefManager &bookBriefManager;

    ListWidgetStarRecord *listWidgetStarRecord;
};
