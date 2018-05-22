// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <client/core/BookBrief.h>
#include <client/core/KeepRecord.h>

class ListWidgetItemKeepRecord : public QListWidgetItem {
public:
    ListWidgetItemKeepRecord(const BookBrief &book, const KeepRecord &record);
    const BookBrief& getBook() const;
    void update(const BookBrief &book, const KeepRecord &record);

private:
    void setUI();

private:
    BookBrief book;
    KeepRecord record;
};

class ListWidgetKeepRecord : public QListWidget {
    Q_OBJECT

public:
    ListWidgetKeepRecord(QWidget *parent = Q_NULLPTR);
    void add(const BookBrief &book, const KeepRecord &record, int row = 0);
    void update(const BookBrief &book, const KeepRecord &record, int row);

private:
    void setUI();
};
