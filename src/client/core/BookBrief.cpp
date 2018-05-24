// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <client/core/BookBrief.h>

BookBrief::xint BookBrief::getBookid() const {
    return bookid;
}

BookBrief& BookBrief::setBookid(const xint &value) {
    bookid = value;
    return *this;
}

BookBrief::xstring BookBrief::getTitle() const {
    return title;
}

BookBrief& BookBrief::setTitle(const xstring &value) {
    title = value;
    return *this;
}

BookBrief::xstring BookBrief::getAuthor() const {
    return author;
}

BookBrief& BookBrief::setAuthor(const xstring &value) {
    author = value;
    return *this;
}

BookBrief::xstring BookBrief::getISBN() const {
    return ISBN;
}

BookBrief& BookBrief::setISBN(const xstring &value) {
    ISBN = value;
    return *this;
}

BookBrief::xstring BookBrief::getPublisher() const {
    return publisher;
}

BookBrief& BookBrief::setPublisher(const xstring &value) {
    publisher = value;
    return *this;
}

BookBrief::xstring BookBrief::getIntroduction() const {
    return introduction;
}

BookBrief& BookBrief::setIntroduction(const xstring &value) {
    introduction = value;
    return *this;
}

BookBrief::xint BookBrief::getPriority() const {
    return priority;
}

BookBrief& BookBrief::setPriority(const xint &value) {
    priority = value;
    return *this;
}

BookBrief::xint BookBrief::getStarCount() const {
    return starCount;
}

BookBrief& BookBrief::setStarCount(const xint &value) {
    starCount = value;
    return *this;
}

BookBrief::xll BookBrief::getMaxKeepTime() const {
    return maxKeepTime;
}

BookBrief& BookBrief::setMaxKeepTime(const xll &value) {
    maxKeepTime = value;
    return *this;
}

Resource BookBrief::getCover() const {
    return cover;
}

BookBrief& BookBrief::setCover(const Resource &cover) {
    this->cover = cover;
    return *this;
}

BookBrief& BookBrief::cleanCover() {
    cover.clean();
    return *this;
}

BookBrief& BookBrief::setFromPtree(const ptree &pt) {
    setBookid(pt.get<xint>("bookid", 0));
    setTitle(pt.get<xstring>("title", ""));
    setAuthor(pt.get<xstring>("author", ""));
    setISBN(pt.get<xstring>("ISBN", ""));
    setPublisher(pt.get<xstring>("publisher", ""));
    setIntroduction(pt.get<xstring>("introduction", ""));
    setPriority(pt.get<xint>("priority", 0));
    setMaxKeepTime(pt.get<xll>("maxKeepTime", 0));
    setStarCount(pt.get<xint>("starCount", 0));
    return *this;
}

BookBrief BookBrief::fromPtree(const ptree &pt) {
    BookBrief book;
    book.setFromPtree(pt);
    return std::move(book);
}

BookBrief& BookBrief::updateFromPtree(const ptree &pt) {
    auto bookid = pt.get_optional<xint>("bookid");
    auto title = pt.get_optional<xstring>("title");
    auto author = pt.get_optional<xstring>("author");
    auto ISBN = pt.get_optional<xstring>("ISBN");
    auto publisher = pt.get_optional<xstring>("publisher");
    auto introduction = pt.get_optional<xstring>("introduction");
    auto priority = pt.get_optional<xint>("priority");
    auto maxKeepTime = pt.get_optional<xll>("maxKeepTime");
    auto starCount = pt.get_optional<xint>("starCount");

    if (bookid)
        setBookid(*bookid);
    if (title)
        setTitle(*title);
    if (author)
        setAuthor(*author);
    if (ISBN)
        setISBN(*ISBN);
    if (publisher)
        setPublisher(*publisher);
    if (introduction)
        setIntroduction(*introduction);
    if (priority)
        setPriority(*priority);
    if (maxKeepTime)
        setMaxKeepTime(*maxKeepTime);
    if (starCount)
        setStarCount(*starCount);
    return *this;
}

const BookBrief& BookBrief::unknown() {
    static BookBrief book;
    static bool init {false};
    if (!init) {
        book.setTitle("Unknown");
        init = true;
    }
    return book;
}
