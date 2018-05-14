// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <client/usermanager.h>
#include <client/xclient.h>

class DialogModify : public QDialog {
    Q_OBJECT

public:
    DialogModify(UserManager &userManager, QWidget *parent);

public slots:
    void slotModifyBegin();
    void slotModifyEnd(const int &ec);

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;

    QLineEdit *editNickname;
    QLineEdit *editEmail;
    QLineEdit *editPasswordOld;
    QLineEdit *editPasswordNew;
    QLineEdit *editPasswordNewConfirm;

    QLabel *labelMessage;

    QDialogButtonBox *btns;
};
