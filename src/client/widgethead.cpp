// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <client/dialogmodify.h>
#include <client/widgethead.h>

WidgetHead::WidgetHead(UserManager &userManager, QWidget *parent) : 
    userManager(userManager),
    QWidget(parent) {

    lblNickname = new QLabel(this);
    btnModify = new QPushButton(this);

    setUI();
    setConnection();
}

WidgetHead::~WidgetHead() {

}

void WidgetHead::slotModify() {
    DialogModify dialog(userManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        lblNickname->setText(tr("Hello, ") + QString::fromStdString(userManager.getUser()->getNickname()));
    }
}

void WidgetHead::setUI() {
    lblNickname->setText(tr("Hello, ") + QString::fromStdString(userManager.getUser()->getNickname()));
    lblNickname->setWordWrap(true);
    btnModify->setText(tr("Modify"));

    auto layoutRight = new QVBoxLayout;
    layoutRight->addStretch();
    layoutRight->addWidget(btnModify);

    auto layout = new QHBoxLayout;
    layout->addWidget(lblNickname);
    layout->addLayout(layoutRight);

    setLayout(layout);
}

void WidgetHead::setConnection() {
    connect(
        btnModify,
        &QPushButton::clicked,
        this,
        &WidgetHead::slotModify
    );
}
