// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <core/bookcore.h>
#include <core/starrecord.h>

class ListWidgetStarRecord : public QListWidget {
    Q_OBJECT

public:
    ListWidgetStarRecord(QWidget *parent = Q_NULLPTR);
    int add(const BookCore &book, const StarRecord &record);

private:
    void setUI();
};
