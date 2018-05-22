// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <client/core/BookBrief.h>
#include <client/core/BorrowRecord.h>

class ListWidgetItemBorrowRecord : public QListWidgetItem {
public:
    ListWidgetItemBorrowRecord(const BookBrief &book, const BorrowRecord &record);
    const BookBrief& getBook() const;
    void update(const BookBrief &book, const BorrowRecord &record);

private:
    void setUI();

private:
    BookBrief book;
    BorrowRecord record;
};

class ListWidgetBorrowRecord : public QListWidget {
    Q_OBJECT

public:
    ListWidgetBorrowRecord(QWidget *parent = Q_NULLPTR);
    void add(const BookBrief &book, const BorrowRecord &record, int row = 0);
    void update(const BookBrief &book, const BorrowRecord &record, int row);

private:
    void setUI();
};
