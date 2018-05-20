// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QObject>
#include <QPixmap>

#include <client/listwidget/ListWidgetBorrowRecord.h>
#include <core/utils.h>

ListWidgetItemBorrowRecord::ListWidgetItemBorrowRecord(const BookBrief &book, const BorrowRecord &record) :
    book(book),
    record(record) {

    setUI();
}

const BookBrief& ListWidgetItemBorrowRecord::getBook() const {
    return book;
}

const BorrowRecord& ListWidgetItemBorrowRecord::getRecord() const {
    return record;
}

void ListWidgetItemBorrowRecord::update(const BookBrief &book, const BorrowRecord &record) {
    this->book = book;
    this->record = record;
    setUI();
}

void ListWidgetItemBorrowRecord::setUI() {
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
            QString::fromStdString(X::time_to_str(record.getEndTime())) +
            QObject::tr("\nreturnTime: ") +
            QString::fromStdString(X::time_to_str(record.getReturnTime())));
}

ListWidgetBorrowRecord::ListWidgetBorrowRecord(QWidget *parent) :
    QListWidget(parent) {

    setUI();
}

void ListWidgetBorrowRecord::add(const BookBrief &book, const BorrowRecord &record, int row) {
    insertItem(row, new ListWidgetItemBorrowRecord(book, record));
}

void ListWidgetBorrowRecord::update(const BookBrief &book, const BorrowRecord &record, int row) {
    auto it = dynamic_cast<ListWidgetItemBorrowRecord *> (item(row));
    it->update(book, record);
}

void ListWidgetBorrowRecord::setUI() {
    setMovement(QListView::Static);
    setIconSize(QSize(114, 160));
    setUniformItemSizes(true);
    setResizeMode(QListWidget::Adjust);
}