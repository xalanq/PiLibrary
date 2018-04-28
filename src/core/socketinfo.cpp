#include "socketinfo.h"

const char SocketInfo::IDENTIFIER = -101;

SocketInfo::SocketInfo() {
    size = HEADER_SIZE;
    buffer = new char[size];
}

SocketInfo::~SocketInfo() {
    delete[] buffer;
}

void SocketInfo::setSize(const X::uint &size) {
    if (this->size < size) {
        delete[] buffer;
        buffer = new char[size];
    }
}

int SocketInfo::getSize() const {
    return size;
}

X::ull SocketInfo::decodeHeaderToken() const {
    return *static_cast<const X::ull *> ((const void *)(buffer));
}

void SocketInfo::encodeHeaderToken(const X::ull &token) {
    memcpy(buffer + 1, (const void *)(&token), sizeof(X::ull));
}

X::uint SocketInfo::decodeHeaderLength() const {
    return *static_cast<const X::uint *> ((const void *)(buffer + sizeof(X::ull)));
}

void SocketInfo::encodeHeaderLength(const X::uint &length) {
    memcpy(buffer + 1 + sizeof(X::ull), (const void *)(&length), sizeof(X::uint));
}

void SocketInfo::encodeIndentifier() {
    buffer[0] = IDENTIFIER;
}

void SocketInfo::encodeHeader(const X::ull &token, const X::uint &length) {
    encodeIndentifier();
    encodeHeaderToken(token);
    encodeHeaderLength(length);
}

X::string SocketInfo::encodePtree(const boost::property_tree::ptree &pt) {
    std::stringstream ss;
    boost::property_tree::write_json(ss, pt, false);
    return ss.str();
}

void SocketInfo::encodeBody(const X::string &str) {
    memcpy(buffer, str.c_str(), str.size());
}

void SocketInfo::decodeBody(const X::uint &length, boost::property_tree::ptree &pt) const {
    std::stringstream ss;
    const std::string str(buffer, length);
    ss << str;

    boost::property_tree::read_json(ss, pt);
}

const char *SocketInfo::getBuffer() const {
    return buffer;
}

char *SocketInfo::getBuffer() {
    return buffer;
}

