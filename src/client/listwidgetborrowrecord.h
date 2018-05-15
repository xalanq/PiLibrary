// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <core/bookbrief.h>
#include <core/borrowrecord.h>

class ListWidgetBorrowRecord : public QListWidget {
    Q_OBJECT

public:
    ListWidgetBorrowRecord(QWidget *parent = Q_NULLPTR);
    void add(const BookBrief &book, const BorrowRecord &record);

private:
    void setUI();
};
