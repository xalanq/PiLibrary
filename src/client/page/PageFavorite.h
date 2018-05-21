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
    void updateStar(const X::xint &bookid, bool star);

signals:
    void signalReady();

public slots:
    void slotGetStarRecord(const std::vector<StarRecord> &records);
    void slotItemClicked(QListWidgetItem *item);

    void refresh();

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;
    BookManager &bookManager;

    ListWidgetStarRecord *listWidgetStarRecord;
};
