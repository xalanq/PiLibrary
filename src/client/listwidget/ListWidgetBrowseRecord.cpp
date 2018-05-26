// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QPixmap>

#include <client/listwidget/ListWidgetBrowseRecord.h>
#include <client/utils.h>

ListWidgetItemBrowseRecord::ListWidgetItemBrowseRecord(const BookBrief &book, const BrowseRecord &record) :
    book(book),
    record(record) {

    setUI();
}

const BookBrief& ListWidgetItemBrowseRecord::getBook() const {
    return book;
}

void ListWidgetItemBrowseRecord::update(const BookBrief &book, const BrowseRecord &record) {
    this->book = book;
    this->record = record;
    setUI();
}

void ListWidgetItemBrowseRecord::setUI() {
    QPixmap p(QSize(114, 160));
    if (book.getCover().getSize())
        p.loadFromData((uchar *)book.getCover().getData(), book.getCover().getSize());
    else
        p.fill(Qt::black);

    QIcon icon;
    icon.addPixmap(p.scaled(QSize(114, 160)));
    setIcon(icon);

    setText(QObject::tr("Title: ") +
            QString::fromStdString(book.getTitle()) +
            QObject::tr("\nTime: ") +
            QString::fromStdString(X::time_to_str(record.getTime())));
}

ListWidgetBrowseRecord::ListWidgetBrowseRecord(QWidget *parent) :
    QListWidget(parent) {

    setUI();
}

void ListWidgetBrowseRecord::add(const BookBrief &book, const BrowseRecord &record, int row) {
    insertItem(row, new ListWidgetItemBrowseRecord(book, record));
}

void ListWidgetBrowseRecord::update(const BookBrief &book, const BrowseRecord &record, int row) {
    auto it = dynamic_cast<ListWidgetItemBrowseRecord *> (item(row));
    if (it)
        it->update(book, record);
}

void ListWidgetBrowseRecord::setUI() {
    setMovement(QListView::Static);
    setIconSize(QSize(114, 160));
    setUniformItemSizes(true);
    setResizeMode(QListWidget::Adjust);
}
