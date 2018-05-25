// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <client/widget/WidgetSearchBook.h>

WidgetSearchBook::WidgetSearchBook(QWidget *parent) :
    QWidget(parent) {

    lblTitle = new QLabel(this);
    lblAuthor = new QLabel(this);
    lblIntroduction = new QLabel(this);
    lblPosition = new QLabel(this);
    lblBookid = new QLabel(this);
    lblPublisher = new QLabel(this);
    lblISBN = new QLabel(this);

    editTitle = new QLineEdit(this);
    editAuthor = new QLineEdit(this);
    editIntroduction = new QLineEdit(this);
    editPosition = new QLineEdit(this);
    editBookid = new QLineEdit(this);
    editPublisher = new QLineEdit(this);
    editISBN = new QLineEdit(this);

    btnSearch = new QPushButton(this);
    btnMore = new QPushButton(this);

    setUI();
    setConnection();
}

void WidgetSearchBook::slotSearch() {
    X::ptree pt;
    auto title = editTitle->text();
    auto author = editAuthor->text();
    auto introduction = editIntroduction->text();
    auto position = editPosition->text();
    auto bookid = editBookid->text();
    auto publisher = editPublisher->text();
    auto ISBN = editISBN->text();
    if (title.size())
        pt.put("title", title.toStdString());
    if (author.size())
        pt.put("author", author.toStdString());
    if (introduction.size())
        pt.put("introduction", introduction.toStdString());
    if (position.size())
        pt.put("position", position.toStdString());
    if (bookid.size())
        pt.put("bookid", bookid.toStdString());
    if (publisher.size())
        pt.put("publisher", publisher.toStdString());
    if (ISBN.size())
        pt.put("ISBN", ISBN.toStdString());

    emit searchInfo(pt);
}

void WidgetSearchBook::slotMore() {
    if (lblAuthor->isHidden()) {
        lblAuthor->show();
        editAuthor->show();
        lblIntroduction->show();
        editIntroduction->show();
        lblPosition->show();
        editPosition->show();
        lblPublisher->show();
        editPublisher->show();
        lblBookid->show();
        editBookid->show();
        lblPublisher->show();
        editPublisher->show();
        lblISBN->show();
        editISBN->show();
    } else {
        lblAuthor->hide();
        editAuthor->hide();
        lblIntroduction->hide();
        editIntroduction->hide();
        lblPosition->hide();
        editPosition->hide();
        lblPublisher->hide();
        editPublisher->hide();
        lblBookid->hide();
        editBookid->hide();
        lblPublisher->hide();
        editPublisher->hide();
        lblISBN->hide();
        editISBN->hide();
    }
}

void WidgetSearchBook::setUI() {
    int maxWidth = 80;

    auto layout = new QVBoxLayout;

    auto layoutTitle = new QHBoxLayout;
    lblTitle->setText(tr("Title: "));
    lblTitle->setFixedWidth(maxWidth);
    lblTitle->setBuddy(editTitle);
    btnSearch->setText(tr("&Search"));
    btnMore->setText(tr("&More"));
    layoutTitle->addWidget(lblTitle);
    layoutTitle->addWidget(editTitle);
    layoutTitle->addWidget(btnSearch);
    layoutTitle->addWidget(btnMore);
    layout->addLayout(layoutTitle);

    auto layoutAuthor = new QHBoxLayout;
    lblAuthor->setText(tr("Author: "));
    lblAuthor->setFixedWidth(maxWidth);
    lblAuthor->setBuddy(editAuthor);
    lblAuthor->hide();
    editAuthor->hide();
    layoutAuthor->addWidget(lblAuthor);
    layoutAuthor->addWidget(editAuthor);
    layout->addLayout(layoutAuthor);

    auto layoutIntroduction = new QHBoxLayout;
    lblIntroduction->setText(tr("Introduction: "));
    lblIntroduction->setFixedWidth(maxWidth);
    lblIntroduction->setBuddy(editIntroduction);
    lblIntroduction->hide();
    editIntroduction->hide();
    layoutIntroduction->addWidget(lblIntroduction);
    layoutIntroduction->addWidget(editIntroduction);
    layout->addLayout(layoutIntroduction);

    auto layoutPosition = new QHBoxLayout;
    lblPosition->setText(tr("Position: "));
    lblPosition->setFixedWidth(maxWidth);
    lblPosition->setBuddy(editPosition);
    lblPosition->hide();
    editPosition->hide();
    layoutPosition->addWidget(lblPosition);
    layoutPosition->addWidget(editPosition);
    layout->addLayout(layoutPosition);

    auto layoutBookid = new QHBoxLayout;
    lblBookid->setText(tr("Bookid: "));
    lblBookid->setFixedWidth(maxWidth);
    lblBookid->setBuddy(editBookid);
    lblBookid->hide();
    editBookid->hide();
    layoutBookid->addWidget(lblBookid);
    layoutBookid->addWidget(editBookid);
    layout->addLayout(layoutBookid);

    auto layoutPublisher = new QHBoxLayout;
    lblPublisher->setText(tr("Publisher: "));
    lblPublisher->setFixedWidth(maxWidth);
    lblPublisher->setBuddy(editPublisher);
    lblPublisher->hide();
    editPublisher->hide();
    layoutPublisher->addWidget(lblPublisher);
    layoutPublisher->addWidget(editPublisher);
    layout->addLayout(layoutPublisher);

    auto layoutISBN = new QHBoxLayout;
    lblISBN->setText(tr("ISBN: "));
    lblISBN->setFixedWidth(maxWidth);
    lblISBN->setBuddy(editISBN);
    lblISBN->hide();
    editISBN->hide();
    layoutISBN->addWidget(lblISBN);
    layoutISBN->addWidget(editISBN);
    layout->addLayout(layoutISBN);

    setLayout(layout);
}

void WidgetSearchBook::setConnection() {
    connect(btnSearch, &QPushButton::clicked, this, &WidgetSearchBook::slotSearch);
    connect(btnMore, &QPushButton::clicked, this, &WidgetSearchBook::slotMore);
}
