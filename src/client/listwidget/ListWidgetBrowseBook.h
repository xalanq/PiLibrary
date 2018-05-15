// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <client/core/BookBrief.h>

class ListWidgetBrowseBook : public QListWidget {
    Q_OBJECT

public:
    ListWidgetBrowseBook(QWidget *parent = Q_NULLPTR);
    void add(const BookBrief &book);

private:
    void setUI();
};
