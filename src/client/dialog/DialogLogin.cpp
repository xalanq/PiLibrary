// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>

#include <client/dialog/DialogLogin.h>

DialogLogin::DialogLogin(UserManager &userManager, QWidget *parent) :
    QDialog(parent) {

    w = new WidgetLogin(userManager, this);

    setUI();
    setConnection();
}

void DialogLogin::setUI() {
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
    
    setWindowFlags(Qt::FramelessWindowHint);

    auto layout = new QHBoxLayout(this);
    layout->addWidget(w);

    setLayout(layout);

    setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(9.0);
    shadow->setColor(QColor(0, 0, 0, 160));
    shadow->setOffset(0, 0);
    setGraphicsEffect(shadow);
}

void DialogLogin::setConnection() {
    connect(w, &WidgetLogin::signalAccept, this, &QDialog::accept);
}
