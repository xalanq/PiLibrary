// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QVBoxLayout>

#include <client/page/PageSetting.h>

PageSetting::PageSetting(UserManager &userManager, QWidget *parent) :
    userManager(userManager),
    QWidget(parent) {

    btnRefresh = new QPushButton(this);

    setUI();
    setConnection();
}

void PageSetting::setUI() {
    btnRefresh->setText(tr("Refresh cache"));

    auto layout = new QVBoxLayout;
    layout->addWidget(btnRefresh);

    setLayout(layout);
}

void PageSetting::setConnection() {
    connect(btnRefresh, SIGNAL(clicked()), this, SIGNAL(signalRefresh()));
}
