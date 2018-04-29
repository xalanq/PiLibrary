#ifndef BOOKCORE_H
#define BOOKCORE_H

#include "xcore.h"

class BookCore {
public:
    virtual ~BookCore() = 0;
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;

    uint getBookid() const;
    void setBookid(const uint &value);

    string getTitle() const;
    void setTitle(const string &value);

    string getAuthor() const;
    void setAuthor(const string &value);

    string getISBN() const;
    void setISBN(const string &value);

    string getPublisher() const;
    void setPublisher(const string &value);

    uint getAmount() const;
    void setAmount(const uint &value);

    string getIntroduction() const;
    void setIntroduction(const string &value);

    uint getPriority() const;
    void setPriority(const uint &value);

private:
    uint bookid;
    string title;
    string author;
    string ISBN;
    string publisher;
    uint amount;
    string introduction;
    uint priority;
};

#endif // BOOKCORE_H
