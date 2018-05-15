// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <core/bookbrief.h>

class ListWidgetBook : public QListWidget {
    Q_OBJECT

public:
    ListWidgetBook(QWidget *parent = Q_NULLPTR);
    void add(const BookBrief &book);

private:
    void setUI();
};
