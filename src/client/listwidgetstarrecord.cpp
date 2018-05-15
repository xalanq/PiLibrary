// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QPixmap>

#include <client/listwidgetstarrecord.h>

ListWidgetStarRecord::ListWidgetStarRecord(QWidget *parent) :
    QListWidget(parent) {

    setUI();
}

int ListWidgetStarRecord::add(const BookBrief &book, const StarRecord &record) {
    QPixmap p(QSize(114, 160));
    QIcon icon;
    p.fill(Qt::black);
    icon.addPixmap(p);
    auto item = new QListWidgetItem(icon,
                                    tr("Title: ") +
                                    QString::fromStdString(book.getTitle()) +
                                    tr("\nTime: ") +
                                    QString::fromStdString(X::time_to_str(record.getTime())));
    addItem(item);
    return count() - 1;
}

void ListWidgetStarRecord::setUI() {
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    setIconSize(QSize(114, 160));
    setUniformItemSizes(true);
    setResizeMode(QListWidget::Adjust);
}
