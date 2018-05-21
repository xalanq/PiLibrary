// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <ctime>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <client/dialog/DialogChooseTime.h>
#include <core/utils.h>

DialogChooseTime::DialogChooseTime(const X::xll &maxKeepTime, QWidget *parent) :
    maxKeepTime(maxKeepTime),
    QDialog(parent) {

    lblBeginTime = new QLabel(this);
    lblEndTime = new QLabel(this);
    lblMaxKeepTime = new QLabel(this);
    lblWarning = new QLabel(this);
    lblError = new QLabel(this);
    lblDay = new QLabel(this);
    lblHour = new QLabel(this);
    lblMinute = new QLabel(this);

    spinDay = new QSpinBox(this);
    spinHour = new QSpinBox(this);
    spinMinute = new QSpinBox(this);

    btns = new QDialogButtonBox(this);

    setUI();
    setConnection();
}

X::xll DialogChooseTime::getKeepTime() const {
    return keepTime;
}

void DialogChooseTime::slotGo() {
    if (keepTime > maxKeepTime) {
        lblError->show();
    } else {
        accept();
    }
}

void DialogChooseTime::slotChanged(int t) {
    X::xll d = spinDay->value();
    X::xll h = spinHour->value();
    X::xll m = spinMinute->value();

    (keepTime *= 0 ) += d;
    (keepTime *= 24) += h;
    (keepTime *= 60) += m;
    (keepTime *= 60);

    lblError->hide();
    lblEndTime->setText(tr("End: ") + QString::fromStdString(X::time_to_str(beginTime + keepTime)));
}

void DialogChooseTime::setUI() {
    setWindowTitle(tr("Choose time"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    beginTime = time(0);
    X::xll d = maxKeepTime / 60 / 60 / 24;
    X::xll h = maxKeepTime / 60 / 60 - d * 24;
    X::xll m = maxKeepTime / 60 - d * 24 * 60 - h * 60;

    lblBeginTime->setText(tr("Begin: ") + QString::fromStdString(X::time_to_str(beginTime)));
    lblEndTime->setText(tr("End: ") + QString::fromStdString(X::time_to_str(beginTime + maxKeepTime)));
    lblMaxKeepTime->setText(tr("Max keep time: ") + QString::number(d) + tr(" d ") + QString::number(h) + tr(" h ") + QString::number(m) + tr(" m "));
    lblWarning->setText(tr("Ensure you want to borrow this book. \nNote: you can not return this book yourself."));
    lblError->setText(tr("<font color='red'>Warning: Invalid Time</font>"));
    lblError->hide();
    lblDay->setText("d");
    lblHour->setText("h");
    lblMinute->setText("m");

    spinDay->setRange(0, 150 * 365);
    spinHour->setRange(0, 59);
    spinMinute->setRange(0, 59);
    spinDay->setValue(d);
    spinHour->setValue(h);
    spinMinute->setValue(m);

    btns->addButton(QDialogButtonBox::Ok)->setText(tr("&Borrow"));
    btns->addButton(QDialogButtonBox::Cancel)->setText(tr("&Cancel"));
    btns->setCenterButtons(true);

    auto layout = new QVBoxLayout;
    auto layoutDay = new QVBoxLayout;
    auto layoutHour = new QVBoxLayout;
    auto layoutMinute = new QVBoxLayout;
    auto layoutSpin = new QHBoxLayout;

    layoutDay->addWidget(lblDay);
    layoutDay->addWidget(spinDay);
    layoutHour->addWidget(lblHour);
    layoutHour->addWidget(spinHour);
    layoutMinute->addWidget(lblMinute);
    layoutMinute->addWidget(spinMinute);

    layoutSpin->addLayout(layoutDay);
    layoutSpin->addLayout(layoutHour);
    layoutSpin->addLayout(layoutMinute);

    layout->addWidget(lblBeginTime);
    layout->addWidget(lblEndTime);
    layout->addWidget(lblMaxKeepTime);
    layout->addLayout(layoutSpin);
    layout->addWidget(lblWarning);
    layout->addWidget(lblError);
    layout->addWidget(btns);

    setLayout(layout);
}

void DialogChooseTime::setConnection() {
    connect(
        btns->button(QDialogButtonBox::Ok),
        &QPushButton::clicked,
        this,
        &DialogChooseTime::slotGo
    );

    connect(
        spinDay,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this,
        &DialogChooseTime::slotChanged
    );

    connect(
        spinHour,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this,
        &DialogChooseTime::slotChanged
    );

    connect(
        spinMinute,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this,
        &DialogChooseTime::slotChanged
    );
}
