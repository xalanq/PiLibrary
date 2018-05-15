// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QPixmap>

#include <client/listwidget/ListWidgetBrowseBook.h>

ListWidgetBrowseBook::ListWidgetBrowseBook(QWidget *parent) :
    QListWidget(parent) {

    setUI();
}

void ListWidgetBrowseBook::add(const BookBrief &book) {
    QPixmap p(QSize(114, 160));
    QIcon icon;
    p.fill(Qt::black);
    icon.addPixmap(p);
    auto item = new QListWidgetItem(icon, QString::fromStdString(book.getTitle() + "\n" + book.getAuthor()));
    addItem(item);
}

void ListWidgetBrowseBook::setUI() {
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    setIconSize(QSize(114, 160));
    setUniformItemSizes(true);
    setResizeMode(QListWidget::Adjust);
}
