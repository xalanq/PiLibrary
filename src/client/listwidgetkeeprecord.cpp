// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QPixmap>

#include <client/listwidgetkeeprecord.h>

ListWidgetKeepRecord::ListWidgetKeepRecord(QWidget *parent) :
    QListWidget(parent) {

    setUI();
}

void ListWidgetKeepRecord::add(const BookCore &book, const KeepRecord &record) {
    QPixmap p(QSize(114, 160));
    QIcon icon;
    p.fill(Qt::black);
    icon.addPixmap(p);
    auto item = new QListWidgetItem(icon,
                                    tr("Title: ") +
                                    QString::fromStdString(book.getTitle()) +
                                    tr("\nbeginTime: ") +
                                    QString::fromStdString(X::time_to_str(record.getBeginTime())) +
                                    tr("\nendTime: ") +
                                    QString::fromStdString(X::time_to_str(record.getEndTime())));
    addItem(item);
}

void ListWidgetKeepRecord::setUI() {
    setMovement(QListView::Static);
    setIconSize(QSize(114, 160));
    setUniformItemSizes(true);
    setResizeMode(QListWidget::Adjust);
}
