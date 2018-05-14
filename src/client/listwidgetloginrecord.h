// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QListWidget>
#include <QWidget>

#include <core/loginrecord.h>

class ListWidgetLoginRecord : public QListWidget {
    Q_OBJECT

public:
    ListWidgetLoginRecord(QWidget *parent = Q_NULLPTR);
    void add(const LoginRecord &record);

private:
    void setUI();
};
