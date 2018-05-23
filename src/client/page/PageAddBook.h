// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QWidget>

#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>
#include <client/widget/WidgetSetBook.h>

class PageAddBook : public WidgetSetBook {
    Q_OBJECT

public:
    PageAddBook(UserManager &userManager, QWidget *parent = Q_NULLPTR);

private:
    void setUI();
};
