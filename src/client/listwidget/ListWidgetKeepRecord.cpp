// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QPixmap>

#include <client/listwidget/ListWidgetKeepRecord.h>
#include <core/utils.h>

ListWidgetItemKeepRecord::ListWidgetItemKeepRecord(const BookBrief &book, const KeepRecord &record) :
    book(book),
    record(record) {

    setUI();
}

const BookBrief& ListWidgetItemKeepRecord::getBook() const {
    return book;
}

const KeepRecord& ListWidgetItemKeepRecord::getRecord() const {
    return record;
}

void ListWidgetItemKeepRecord::update(const BookBrief &book, const KeepRecord &record) {
    this->book = book;
    this->record = record;
    setUI();
}

void ListWidgetItemKeepRecord::setUI() {
    QPixmap p(QSize(114, 160));
    QIcon icon;
    p.fill(Qt::black);
    icon.addPixmap(p);
    setIcon(icon);
    setText(QObject::tr("Title: ") +
            QString::fromStdString(book.getTitle()) +
            QObject::tr("\nbeginTime: ") +
            QString::fromStdString(X::time_to_str(record.getBeginTime())) +
            QObject::tr("\nendTime: ") +
            QString::fromStdString(X::time_to_str(record.getEndTime())));
}

ListWidgetKeepRecord::ListWidgetKeepRecord(QWidget *parent) :
    QListWidget(parent) {

    setUI();
}

void ListWidgetKeepRecord::add(const BookBrief &book, const KeepRecord &record, int row) {
    insertItem(row, new ListWidgetItemKeepRecord(book, record));
}

void ListWidgetKeepRecord::update(const BookBrief &book, const KeepRecord &record, int row) {
    auto it = dynamic_cast<ListWidgetItemKeepRecord *> (item(row));
    it->update(book, record);
}

void ListWidgetKeepRecord::setUI() {
    setMovement(QListView::Static);
    setIconSize(QSize(114, 160));
    setUniformItemSizes(true);
    setResizeMode(QListWidget::Adjust);
}
