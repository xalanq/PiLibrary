// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QGroupBox>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <client/dialog/DialogModifyUser.h>
#include <client/page/PageSetting.h>

PageSetting::PageSetting(UserManager &userManager, QWidget *parent) :
    userManager(userManager),
    QWidget(parent) {

    btnRefresh = new QPushButton(this);
    btnModify = new QPushButton(this);
    btnLogout = new QPushButton(this);

    setUI();
    setConnection();
}

void PageSetting::slotModify() {
    DialogModifyUser dialog(userManager, this);
    if (dialog.exec() == QDialog::Accepted)
        emit signalModifyUser();
}

void PageSetting::slotLogout() {
    auto res = QMessageBox::information(this, tr("Logout"), tr("Are you sure??"), QMessageBox::Ok, QMessageBox::Cancel);
    if (res == QMessageBox::Ok)
        emit signalLogout();
}

void PageSetting::setUI() {
    auto groupUser = new QGroupBox(tr("User"), this);
    btnModify->setText(tr("Modify"));
    btnLogout->setText(tr("Logout"));

    auto layoutUser = new QHBoxLayout;
    layoutUser->addWidget(btnModify);
    layoutUser->addWidget(btnLogout);
    layoutUser->addStretch();

    groupUser->setLayout(layoutUser);


    auto groupBook = new QGroupBox(tr("Book"), this);

    btnRefresh->setText(tr("Refresh cache"));
    auto layoutBook = new QHBoxLayout;
    layoutBook->addWidget(btnRefresh);
    layoutBook->addStretch();

    groupBook->setLayout(layoutBook);


    auto layout = new QVBoxLayout;
    layout->addWidget(groupUser);
    layout->addWidget(groupBook);
    layout->addStretch();
    setLayout(layout);
}

void PageSetting::setConnection() {
    connect(btnRefresh, SIGNAL(clicked()), this, SIGNAL(signalRefresh()));
    connect(btnModify, &QPushButton::clicked, this, &PageSetting::slotModify);
    connect(btnLogout, &QPushButton::clicked, this, &PageSetting::slotLogout);
}
