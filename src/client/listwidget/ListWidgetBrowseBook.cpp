// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QIcon>
#include <QPainter>
#include <QPixmap>

#include <client/listwidget/ListWidgetBrowseBook.h>

ListWidgetItemBook::ListWidgetItemBook(const BookBrief &book) :
    book(book) {

    setUI();
}

const BookBrief& ListWidgetItemBook::getBook() const {
    return book;
}

void ListWidgetItemBook::setUI() {
    QPixmap p(QSize(114, 160));
    p.fill(Qt::black);

    QString str;
    X::xint cnt = book.getStarCount();
    if (cnt > 10000)
        str = QString::fromWCharArray(L"❤ ") + QString::number(cnt / 1000) + QObject::tr("k");
    else
        str = QString::fromWCharArray(L"❤ ") + QString::number(cnt);

    QFont font;
    font.setFamily("Helvetica");
    font.setPointSize(10);

    QPainter painter(&p);
    painter.setPen(Qt::white);
    painter.setFont(font);
    painter.drawText(QPoint(5, 25), str);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setCompositionMode(QPainter::CompositionMode_Xor);

    QIcon icon;
    icon.addPixmap(p);

    setIcon(icon);
    setText(QString::fromStdString(book.getTitle() + '\n' + book.getAuthor()));
}

ListWidgetBrowseBook::ListWidgetBrowseBook(QWidget *parent) :
    QListWidget(parent) {

    setUI();
}

void ListWidgetBrowseBook::add(const BookBrief &book) {
    addItem(new ListWidgetItemBook(book));
}

void ListWidgetBrowseBook::setUI() {
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    setIconSize(QSize(114, 160));
    setUniformItemSizes(true);
    setResizeMode(QListWidget::Adjust);
}
