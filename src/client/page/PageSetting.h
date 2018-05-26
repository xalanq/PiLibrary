// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QStringList>
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
    void slotChangeLanguage(int index);

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;
    
    QPushButton *btnModify;
    QPushButton *btnLogout;

    QPushButton *btnRefresh;

    QComboBox *cbboxLanguage;
    QLabel *lblLanguage;

    QStringList languageList;
    QStringList languageFileName;
};
