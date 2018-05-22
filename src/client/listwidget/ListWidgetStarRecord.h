// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <client/core/BookBrief.h>
#include <client/core/StarRecord.h>

class ListWidgetItemStarRecord : public QListWidgetItem {
public:
    ListWidgetItemStarRecord(const BookBrief &book, const StarRecord &record);
    const BookBrief& getBook() const;
    void update(const BookBrief &book, const StarRecord &record);

private:
    void setUI();

private:
    BookBrief book;
    StarRecord record;
};

class ListWidgetStarRecord : public QListWidget {
    Q_OBJECT

public:
    ListWidgetStarRecord(QWidget *parent = Q_NULLPTR);
    void add(const BookBrief &book, const StarRecord &record, int row = 0);
    void update(const BookBrief &book, const StarRecord &record, int row);

private:
    void setUI();
};
