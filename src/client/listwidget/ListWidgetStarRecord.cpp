// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QPixmap>

#include <client/listwidget/ListWidgetStarRecord.h>
#include <core/utils.h>

ListWidgetItemStarRecord::ListWidgetItemStarRecord(const BookBrief &book, const StarRecord &record) :
    book(book),
    record(record) {

    setUI();
}

const BookBrief& ListWidgetItemStarRecord::getBook() const {
    return book;
}

const StarRecord& ListWidgetItemStarRecord::getRecord() const {
    return record;
}

void ListWidgetItemStarRecord::update(const BookBrief &book, const StarRecord &record) {
    this->book = book;
    this->record = record;
    setUI();
}

void ListWidgetItemStarRecord::setUI() {
    QPixmap p(QSize(114, 160));
    QIcon icon;
    p.fill(Qt::black);
    icon.addPixmap(p);
    setIcon(icon);
    setText(QObject::tr("Title: ") +
            QString::fromStdString(book.getTitle()) +
            QObject::tr("\nTime: ") +
            QString::fromStdString(X::time_to_str(record.getTime())));
}

ListWidgetStarRecord::ListWidgetStarRecord(QWidget *parent) :
    QListWidget(parent) {

    setUI();
}

void ListWidgetStarRecord::add(const BookBrief &book, const StarRecord &record, int row) {
    insertItem(row, new ListWidgetItemStarRecord(book, record));
}

void ListWidgetStarRecord::update(const BookBrief &book, const StarRecord &record, int row) {
    auto it = dynamic_cast<ListWidgetItemStarRecord *> (item(row));
    it->update(book, record);
}

void ListWidgetStarRecord::setUI() {
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    setIconSize(QSize(114, 160));
    setUniformItemSizes(true);
    setResizeMode(QListWidget::Adjust);
}
