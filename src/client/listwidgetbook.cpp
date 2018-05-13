// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QPixmap>

#include <client/listwidgetbook.h>

ListWidgetBook::ListWidgetBook(QWidget *parent) :
    QListWidget(parent) {

    setUI();
}

void ListWidgetBook::addBook(const BookCore &book) {
    QPixmap p;
    QIcon icon;
    icon.addPixmap(p);
    auto item = new QListWidgetItem(icon, QString::fromStdString(book.getTitle()));
    addItem(item);
}

void ListWidgetBook::setUI() {
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    setIconSize(QSize(114, 160));
    setUniformItemSizes(true);
    setResizeMode(QListWidget::Adjust);
}
