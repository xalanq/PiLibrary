// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <client/core/BookBrief.h>
#include <client/core/BrowseRecord.h>

class ListWidgetItemBrowseRecord : public QListWidgetItem {
public:
    ListWidgetItemBrowseRecord(const BookBrief &book, const BrowseRecord &record);
    const BookBrief& getBook() const;
    void update(const BookBrief &book, const BrowseRecord &record);

private:
    void setUI();

private:
    BookBrief book;
    BrowseRecord record;
};

class ListWidgetBrowseRecord : public QListWidget {
    Q_OBJECT

public:
    ListWidgetBrowseRecord(QWidget *parent = Q_NULLPTR);
    void add(const BookBrief &book, const BrowseRecord &record, int row = 0);
    void update(const BookBrief &book, const BrowseRecord &record, int row);

private:
    void setUI();
};
