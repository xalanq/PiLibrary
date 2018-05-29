// copyright 2018 xalanq, chang-ran
// license: lgpl v3.0

#pragma once

#include <map>

#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>

#include <client/manager/BookManager.h>
#include <client/manager/UserManager.h>
#include <client/core/BookBrief.h>
#include <core/types.h>

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
    ListWidgetBrowseBook(UserManager &userManager, BookManager &bookManager, QWidget *parent = Q_NULLPTR);
    void add(const BookBrief &book, bool star, int row = 0);
    void update(const BookBrief &book, bool star, int row);
    void updateStar(const X::xint &bookid, bool star);

signals:
    void signalReady();
    void signalModify();

public slots:
    void slotGetBookList(const X::ErrorCode &ec, const X::ptree &pt, bool reverse = false);
    void slotItemClicked(QListWidgetItem *item);

private:
    void setUI();
    void setConnection();

private:
    UserManager &userManager;
    BookManager &bookManager;

    std::map<X::xint, int> books;
};
