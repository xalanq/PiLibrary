// copyright 2018 xalanq, chang-ran
// license: lgpl v3.0

#pragma once

#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>

#include <client/core/BookBrief.h>

class ListWidgetItemBook : public QListWidgetItem {
public:
    ListWidgetItemBook(const BookBrief &book);
    const BookBrief& getBook() const;

private:
    void setUI();

private:
    BookBrief book;
};

class ListWidgetBrowseBook : public QListWidget {
    Q_OBJECT

public:
    ListWidgetBrowseBook(QWidget *parent = Q_NULLPTR);
    void add(const BookBrief &book);

public slots:

private:
    void setUI();
};
