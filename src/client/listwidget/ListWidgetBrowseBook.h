// copyright 2018 xalanq, chang-ran
// license: lgpl v3.0

#pragma once

#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>

#include <client/core/BookBrief.h>

class ListWidgetItemBook : public QListWidgetItem {
public:
    ListWidgetItemBook(const BookBrief &book, bool star);
    const BookBrief& getBook() const;
    void update(const BookBrief &book, bool star);

private:
    void setUI();

private:
    BookBrief book;
    bool star;
};

class ListWidgetBrowseBook : public QListWidget {
    Q_OBJECT

public:
    ListWidgetBrowseBook(QWidget *parent = Q_NULLPTR);
    void add(const BookBrief &book, bool star, int row = 0);
    void update(const BookBrief &book, bool star, int row);

public slots:

private:
    void setUI();
};
