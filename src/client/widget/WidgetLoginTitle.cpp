// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QFile>
#include <QHBoxLayout>
#include <QMouseEvent>

#include <client/widget/WidgetLoginTitle.h>

WidgetLoginTitle::WidgetLoginTitle(QWidget *moveWidget, QWidget *parent) :
    QWidget(parent) {

    btnMin = new QPushButton(this);
    btnMin->setObjectName("btnMin");
    btnClose = new QPushButton(this);
    btnClose->setObjectName("btnClose");
    this->moveWidget = moveWidget;
    
    setUI();
    setConnection();
}

void WidgetLoginTitle::setUI() {
    static const int W = 50;
    static const int H = 50;

    QFile f(":/style/WidgetLogin/title.css");
    f.open(QFile::ReadOnly);
    if (f.isOpen())
        setStyleSheet(f.readAll());
    
    btnMin->setFixedSize(QSize(W, H));
    btnMin->setFocusPolicy(Qt::NoFocus);
    btnClose->setFixedSize(QSize(W, H));
    btnClose->setFocusPolicy(Qt::NoFocus);
    
    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addStretch();
    layout->addWidget(btnMin);
    layout->addWidget(btnClose);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    setFixedHeight(H);
    setWindowFlags(Qt::FramelessWindowHint);
}

void WidgetLoginTitle::setConnection() {
    connect(btnMin, &QPushButton::clicked, this, &WidgetLoginTitle::slotMin);
    connect(btnClose, &QPushButton::clicked, this, &WidgetLoginTitle::slotClose);
}

void WidgetLoginTitle::mousePressEvent(QMouseEvent *event) {
    isPressed = true;
    startPoint = event->globalPos();
    QWidget::mousePressEvent(event);
}

void WidgetLoginTitle::mouseMoveEvent(QMouseEvent *event) {
    if (isPressed) {
        auto now = event->globalPos();
        auto moveDelta = now - startPoint;
        startPoint = now;
        moveWidget->move(moveWidget->pos() + moveDelta);
    }
    QWidget::mouseMoveEvent(event);
}

void WidgetLoginTitle::mouseReleaseEvent(QMouseEvent *event) {
    isPressed = false;
    QWidget::mouseReleaseEvent(event);
}

void WidgetLoginTitle::slotMin() {
    emit signalMin();
}

void WidgetLoginTitle::slotClose() {
    emit signalClose();
}
