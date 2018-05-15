// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QPixmap>

#include <client/listwidget/ListWidgetBorrowRecord.h>
#include <core/utils.h>

ListWidgetBorrowRecord::ListWidgetBorrowRecord(QWidget *parent) :
    QListWidget(parent) {

    setUI();
}

void ListWidgetBorrowRecord::add(const BookBrief &book, const BorrowRecord &record) {
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
                                    QString::fromStdString(X::time_to_str(record.getEndTime())) +
                                    tr("\nreturnTime: ") +
                                    QString::fromStdString(X::time_to_str(record.getReturnTime())));
    addItem(item);
}

void ListWidgetBorrowRecord::setUI() {
    setMovement(QListView::Static);
    setIconSize(QSize(114, 160));
    setUniformItemSizes(true);
    setResizeMode(QListWidget::Adjust);
}
