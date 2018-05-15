// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <core/bookbrief.h>
#include <core/keeprecord.h>

class ListWidgetKeepRecord : public QListWidget {
    Q_OBJECT

public:
    ListWidgetKeepRecord(QWidget *parent = Q_NULLPTR);
    void add(const BookBrief &book, const KeepRecord &record);

private:
    void setUI();
};
