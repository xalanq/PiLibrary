// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <client/widget/WidgetHead.h>

WidgetHead::WidgetHead(UserManager &userManager, QWidget *parent) : 
    userManager(userManager),
    QWidget(parent) {

    lblNickname = new QLabel(this);

    setUI();
}

void WidgetHead::slotModify() {
    lblNickname->setText(tr("Hello\n") + QString::fromStdString(userManager.getUser().getNickname()));
}

void WidgetHead::setUI() {
    lblNickname->setWordWrap(true);
    lblNickname->setAlignment(Qt::AlignCenter);
    lblNickname->setFixedHeight(100);

    auto layout = new QHBoxLayout;
    layout->addWidget(lblNickname, Qt::AlignCenter);

    setLayout(layout);
    slotModify();
}
