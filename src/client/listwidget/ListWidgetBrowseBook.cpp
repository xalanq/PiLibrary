// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QIcon>
#include <QPainter>
#include <QPixmap>

#include <client/listwidget/ListWidgetBrowseBook.h>

ListWidgetItemBook::ListWidgetItemBook(const BookBrief &book, bool star) :
    book(book),
    star(star) {

    setUI();
}

const BookBrief& ListWidgetItemBook::getBook() const {
    return book;
}

void ListWidgetItemBook::update(const BookBrief &book, bool star) {
    this->book = book;
    this->star = star;
    setUI();
}

void ListWidgetItemBook::setUI() {
    QPixmap p(QSize(114, 160));
    if (book.getCover().getSize())
        p.loadFromData((uchar *)book.getCover().getData(), book.getCover().getSize());
    else
        p.fill(Qt::black);

    QString str = QString::fromWCharArray(L"❤ ");
    X::xint cnt = book.getStarCount();
    if (cnt > 10000)
        str += QString::number(cnt / 1000) + QObject::tr("k");
    else
        str += QString::number(cnt);

    QFont font;
    font.setFamily("Helvetica");
    font.setPointSize(10);

    QPainter painter(&p);
    if (star)
        painter.setPen(Qt::red);
    else
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

void ListWidgetBrowseBook::add(const BookBrief &book, bool star, int row) {
    insertItem(row, new ListWidgetItemBook(book, star));
}

void ListWidgetBrowseBook::update(const BookBrief &book, bool star, int row) {
    auto it = dynamic_cast<ListWidgetItemBook *> (item(row));
    if (it)
        it->update(book, star);
}

void ListWidgetBrowseBook::setUI() {
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    setIconSize(QSize(114, 160));
    setUniformItemSizes(true);
    setResizeMode(QListWidget::Adjust);
}
