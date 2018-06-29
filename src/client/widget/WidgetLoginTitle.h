// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <QPoint>
#include <QPushButton>
#include <QWidget>

#include <client/manager/UserManager.h>

class WidgetLoginTitle : public QWidget {
    Q_OBJECT

public:
    WidgetLoginTitle(QWidget *moveWidget, QWidget *parent = Q_NULLPTR);

signals:
    void signalMin();
    void signalClose();

private slots:
	void slotMin();
    void slotClose();

private:
    void setUI();
    void setConnection();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
	QPushButton *btnMin;
	QPushButton *btnClose;
    QWidget *moveWidget;

    QPoint startPoint;
    bool isPressed {false};
};