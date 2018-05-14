// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <ctime>

#include <client/listwidgetloginrecord.h>

ListWidgetLoginRecord::ListWidgetLoginRecord(QWidget *parent) :
    QListWidget(parent) {

    setUI();
}

void ListWidgetLoginRecord::add(const LoginRecord &record) {
    auto item = new QListWidgetItem(
        tr("IP Address: ") +
        record.getIp().c_str() +
        tr(", \tTime: ") +
        QString::fromStdString(X::time_to_str(record.getTime()))
    );
    addItem(item);
}

void ListWidgetLoginRecord::setUI() {
}
