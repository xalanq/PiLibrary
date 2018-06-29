// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QDialog>

#include <client/widget/WidgetLogin.h>

class DialogLogin : public QDialog {
    Q_OBJECT

public:
    DialogLogin(UserManager &userManager, QWidget *parent = Q_NULLPTR);

private:
    void setUI();
    void setConnection();

private:
    WidgetLogin *w;
};

