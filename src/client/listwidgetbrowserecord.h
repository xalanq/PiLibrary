// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <core/bookbrief.h>
#include <core/browserecord.h>

class ListWidgetBrowseRecord : public QListWidget {
    Q_OBJECT

public:
    ListWidgetBrowseRecord(QWidget *parent = Q_NULLPTR);
    int add(const BookBrief &book, const BrowseRecord &record);

private:
    void setUI();
};
