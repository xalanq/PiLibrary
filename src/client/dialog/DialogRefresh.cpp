// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QApplication>
#include <QDesktopWidget>
#include <QVBoxLayout>

#include <client/dialog/DialogRefresh.h>

DialogRefresh::DialogRefresh(QWidget *parent) :
    QDialog(parent) {

    lblRefresh = new QLabel(this);

    setUI();
}

void DialogRefresh::setUI() {
    setWindowTitle(tr("Refreshing"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    lblRefresh->setText(tr("Refreshing...\nPlease wait a while..."));
    lblRefresh->setAlignment(Qt::AlignCenter);
    lblRefresh->setMargin(50);

    auto layout = new QVBoxLayout;

    layout->addWidget(lblRefresh);
    setLayout(layout);

    adjustSize();
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}
