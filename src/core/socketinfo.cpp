#include "socketinfo.h"

const char SocketInfo::IDENTIFIER[SocketInfo::IDENTIFIER_SIZE + 1] = {85, -41, 19, -117, 0};

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
    return *static_cast<const X::ull *> ((const void *)(buffer + IDENTIFIER_SIZE));
}

void SocketInfo::encodeHeaderToken(const X::ull &token) {
    memcpy(buffer + IDENTIFIER_SIZE, (const void *)(&token), sizeof(X::ull));
}

X::uint SocketInfo::decodeHeaderLength() const {
    return *static_cast<const X::uint *> ((const void *)(buffer + IDENTIFIER_SIZE + sizeof(X::ull)));
}

void SocketInfo::encodeHeaderLength(const X::uint &length) {
    memcpy(buffer + IDENTIFIER_SIZE + sizeof(X::ull), (const void *)(&length), sizeof(X::uint));
}

void SocketInfo::encodeIndentifier() {
    memcpy(buffer, IDENTIFIER, IDENTIFIER_SIZE);
}

void SocketInfo::encodeHeader(const X::ull &token, const X::uint &length) {
    encodeIndentifier();
    encodeHeaderToken(token);
    encodeHeaderLength(length);
}

X::uint SocketInfo::encodeBody(const boost::property_tree::ptree &pt) {
    std::stringstream ss;
    boost::property_tree::write_json(ss, pt, false);
    std::string s = ss.str();
    memcpy(buffer, s.c_str(), s.size());
    return s.size();
}

void SocketInfo::decodeBody(const X::uint &length, boost::property_tree::ptree &pt) const {
    std::stringstream ss;
    const std::string str(buffer, length);
    ss << str;

    boost::property_tree::read_json(ss, pt);
}

bool SocketInfo::matchIdentifier(const size_t &length) const { // can optimize
    for (auto x = matchIndentifierPosition; x + IDENTIFIER_SIZE <= length; ++x) {
        bool flag = 1;
        for (size_t i = 0; i < IDENTIFIER_SIZE && flag; ++i)
            if (buffer[x + i] != IDENTIFIER[i])
                flag = 0;
        if (flag)
            return 1;
    }
    return 0;
}

void SocketInfo::setMatchIndentifierPosition(const size_t &pos) {
    matchIndentifierPosition = pos;
}

const char *SocketInfo::getBuffer() const {
    return buffer;
}

char *SocketInfo::getBuffer() {
    return buffer;
}

