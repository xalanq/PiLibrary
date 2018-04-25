#ifndef BOOKCORE_H
#define BOOKCORE_H

#include "xcore.h"

class BookCore {
public:
    virtual ~BookCore() = 0;

    X::uint getBookid() const;
    void setBookid(const X::uint &value);

    X::string getTitle() const;
    void setTitle(const X::string &value);

    X::string getAuthor() const;
    void setAuthor(const X::string &value);

    X::string getISBN() const;
    void setISBN(const X::string &value);

    X::string getPublisher() const;
    void setPublisher(const X::string &value);

    X::uint getAmount() const;
    void setAmount(const X::uint &value);

    X::string getIntroduction() const;
    void setIntroduction(const X::string &value);

    int getPriority() const;
    void setPriority(int value);

private:
    X::uint bookid;
    X::string title;
    X::string author;
    X::string ISBN;
    X::string publisher;
    X::uint amount;
    X::string introduction;
    int priority;
};

#endif // BOOKCORE_H
