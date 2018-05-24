// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QPushButton>
#include <QWidget>

#include <client/manager/UserManager.h>

class PageSetting : public QWidget {
    Q_OBJECT

public:
    PageSetting(UserManager &userManager, QWidget *parent = Q_NULLPTR);

signals:
    void signalRefresh();
    void signalModifyUser();
    void signalLogout();

public slots:
    void slotModify();
    void slotLogout();

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;
    
    QPushButton *btnRefresh;
    QPushButton *btnModify;
    QPushButton *btnLogout;
};
