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

void ListWidgetItemBorrowRecord::update(const BookBrief &book, const BorrowRecord &record) {
    this->book = book;
    this->record = record;
    setUI();
}

void ListWidgetItemBorrowRecord::setUI() {
    QPixmap p(QSize(114, 160));
    if (book.getCover().getSize())
        p.loadFromData((uchar *)book.getCover().getData(), book.getCover().getSize());
    else
        p.fill(Qt::black);

    QIcon icon;
    icon.addPixmap(p.scaled(QSize(114, 160)));
    setIcon(icon);

    auto beginTime = record.getBeginTime();
    auto endTime = record.getEndTime();
    auto returnTime = record.getReturnTime();
    setText(QObject::tr("Bookid: ") + 
            QString::number(book.getBookid()) +
            QObject::tr("\nTitle: ") +
            QString::fromStdString(book.getTitle()) +
            QObject::tr("\nbeginTime: ") +
            QString::fromStdString(X::time_to_str(beginTime)) +
            QObject::tr("\nendTime: ") +
            QString::fromStdString(X::time_to_str(endTime)) +
            QObject::tr("\nreturnTime: ") +
            (returnTime ? QString::fromStdString(X::time_to_str(returnTime)) : QObject::tr("not returned yet")));
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
    if (it)
        it->update(book, record);
}

void ListWidgetBorrowRecord::setUI() {
    setMovement(QListView::Static);
    setIconSize(QSize(114, 160));
    setUniformItemSizes(true);
    setResizeMode(QListWidget::Adjust);
}