// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <client/core/BookBrief.h>
#include <client/core/BorrowRecord.h>

class ListWidgetBorrowRecord : public QListWidget {
    Q_OBJECT

public:
    ListWidgetBorrowRecord(QWidget *parent = Q_NULLPTR);
    void add(const BookBrief &book, const BorrowRecord &record);

private:
    void setUI();
};
