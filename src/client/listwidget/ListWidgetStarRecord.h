// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <client/core/BookBrief.h>
#include <client/core/StarRecord.h>

class ListWidgetStarRecord : public QListWidget {
    Q_OBJECT

public:
    ListWidgetStarRecord(QWidget *parent = Q_NULLPTR);
    int add(const BookBrief &book, const StarRecord &record);

private:
    void setUI();
};
