// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <core/bookcore.h>

class ListWidgetBook : public QListWidget {
    Q_OBJECT

public:
    ListWidgetBook(QWidget *parent = Q_NULLPTR);
    void addBook(const BookCore &book);

private:
    void setUI();
};
