// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QWidget>

#include <client/manager/UserManager.h>
#include <client/widget/WidgetLoginTitle.h>
#include <core/types.h>

#include "ui_WidgetLogin.h"

class WidgetLogin : public QWidget, private Ui::WidgetLogin {
    Q_OBJECT

public:
    WidgetLogin(UserManager &userManager, QWidget *parent = Q_NULLPTR);
    ~WidgetLogin();
    void loadSetting();
    void saveSetting();
    
signals:
    void signalAccept();

public slots:
    void slotLoginBegin();
    void slotLoginEnd(const X::ErrorCode &ec, const X::xll &token, const X::ptree &pt);
    void slotRegister();

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;

    WidgetLoginTitle *title;
};

