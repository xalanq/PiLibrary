// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <regex>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <client/widget/WidgetSearchBook.h>
#include <core/utils.h>

WidgetSearchBook::WidgetSearchBook(QWidget *parent) :
    QWidget(parent) {

#define XNEW(name) \
    lbl##name = new QLabel(this); \
    edit##name = new QLineEdit(this); \
    regex##name = new QCheckBox(this);

    XNEW(Title)
    XNEW(Author)
    XNEW(Introduction)
    XNEW(Position)
    XNEW(Bookid)
    XNEW(Publisher)
    XNEW(ISBN)

    btnSearch = new QPushButton(this);
    btnMore = new QPushButton(this);

    setUI();
    setConnection();
}

void WidgetSearchBook::slotSearch() {
    X::ptree pt;

#define XPUT(low, big) \
    auto x##low = edit##big->text(); \
    if (x##low.size()) { \
        if (regex##big->isChecked()) \
            pt.put(#low, x##low.toStdString()); \
        else \
            pt.put(#low, X::escape(x##low.toStdString())); \
    }

    XPUT(title, Title)
    XPUT(author, Author)
    XPUT(introduction, Introduction)
    XPUT(position, Position)
    XPUT(bookid, Bookid)
    XPUT(publisher, Publisher)
    XPUT(ISBN, ISBN)

    emit searchInfo(pt);
}

void WidgetSearchBook::slotMore() {
    if (lblAuthor->isHidden()) {
#define XSHOW(name) \
        lbl##name->show(); \
        edit##name->show(); \
        regex##name->show();

        regexTitle->show();
        XSHOW(Author)
        XSHOW(Introduction)
        XSHOW(Position)
        XSHOW(Publisher)
        XSHOW(Bookid)
        XSHOW(ISBN)
    } else {
#define XHIDE(name) \
        lbl##name->hide(); \
        edit##name->hide(); \
        regex##name->hide();

        regexTitle->hide();
        XHIDE(Author)
        XHIDE(Introduction)
        XHIDE(Position)
        XHIDE(Publisher)
        XHIDE(Bookid)
        XHIDE(ISBN)
    }
}

void WidgetSearchBook::setUI() {
    int maxWidth = 130;

    auto layout = new QVBoxLayout;

    auto layoutTitle = new QHBoxLayout;
    lblTitle->setFixedWidth(maxWidth);
    lblTitle->setBuddy(editTitle);
    btnSearch->setText(tr("&Search"));
    btnMore->setText(tr("&More"));
    regexTitle->setText(tr("&Regex"));
    regexTitle->hide();
    layoutTitle->addWidget(lblTitle);
    layoutTitle->addWidget(editTitle);
    layoutTitle->addWidget(btnSearch);
    layoutTitle->addWidget(btnMore);
    layoutTitle->addWidget(regexTitle);
    layout->addLayout(layoutTitle);

#define XSETUI(name) \
    auto layout##name = new QHBoxLayout; \
    lbl##name->setBuddy(edit##name); \
    lbl##name->setFixedWidth(maxWidth); \
    lbl##name->hide(); \
    edit##name->hide(); \
    regex##name->setText(tr("&Regex")); \
    regex##name->hide(); \
    layout##name->addWidget(lbl##name); \
    layout##name->addWidget(edit##name); \
    layout##name->addWidget(regex##name); \
    layout->addLayout(layout##name);

    XSETUI(Author)
    XSETUI(Introduction)
    XSETUI(Position)
    XSETUI(Bookid)
    XSETUI(Publisher)
    XSETUI(ISBN)

    lblTitle->setText(tr("Title: "));
    lblAuthor->setText(tr("&Author: "));
    lblIntroduction->setText(tr("&Introduction: "));
    lblPosition->setText(tr("&Position: "));
    lblBookid->setText(tr("&Bookid: "));
    lblPublisher->setText(tr("&Publisher: "));
    lblISBN->setText(tr("&ISBN: "));

    btnSearch->setDefault(true);

    setLayout(layout);
}

void WidgetSearchBook::setConnection() {
    connect(btnSearch, &QPushButton::clicked, this, &WidgetSearchBook::slotSearch);
    connect(btnMore, &QPushButton::clicked, this, &WidgetSearchBook::slotMore);

#define XENTER(name) \
    connect(##name, &QLineEdit::returnPressed, btnSearch, std::bind(&QPushButton::clicked, btnSearch, true));

    XENTER(editTitle)
    XENTER(editAuthor)
    XENTER(editIntroduction)
    XENTER(editPosition)
    XENTER(editBookid)
    XENTER(editPublisher)
    XENTER(editISBN)
}
