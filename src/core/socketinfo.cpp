#include "socketinfo.h"

const char SocketInfo::IDENTIFIER = -101;

SocketInfo::SocketInfo() {
    size = 0;
    buffer = 0;
}

SocketInfo::~SocketInfo() {
    if (buffer)
        delete[] buffer;
}

void SocketInfo::setSize(const size_t &size) {
    if (this->size < size) {
        if (buffer)
            delete[] buffer;
        buffer = new char[size];
    }
}

size_t SocketInfo::getSize() const {
    return size;
}

SocketInfo::ull SocketInfo::decodeHeaderToken() const {
    return *static_cast<const ull *> ((const void *)(buffer));
}

SocketInfo::uint SocketInfo::decodeHeaderLength() const {
    return *static_cast<const uint *> ((const void *)(buffer + sizeof(ull)));
}

SocketInfo::ActionCode SocketInfo::decodeHeaderActionCode() const {
    return *static_cast<const ActionCode *> ((const void *)(buffer + sizeof(ull) + sizeof(uint)));
}

void SocketInfo::decodeBody(const uint &length, ptree &pt) const {
    std::stringstream ss;
    const std::string str(buffer, length);
    ss << str;
    boost::property_tree::read_json(ss, pt);
}

void SocketInfo::encodeIndentifier() {
    buffer[0] = IDENTIFIER;
}

void SocketInfo::encodeHeaderToken(const ull &token) {
    memcpy(buffer + 1, (const void *)(&token), sizeof(ull));
}

void SocketInfo::encodeHeaderLength(const uint &length) {
    memcpy(buffer + 1 + sizeof(ull), (const void *)(&length), sizeof(uint));
}

void SocketInfo::encodeHeaderActionCode(const ActionCode &ac) {
    memcpy(buffer + 1 + sizeof(ull) + sizeof(uint), (const void *)(&ac), sizeof(ActionCode));
}

void SocketInfo::encodeBody(const string &str) {
    memcpy(buffer + 1 + sizeof(ull) + sizeof(uint) + sizeof(ActionCode), str.c_str(), str.size());
}

void SocketInfo::encode(const ull &token, const uint &bodyLength, const ActionCode &ac, const string &str) {
    encodeIndentifier();
    encodeHeaderToken(token);
    encodeHeaderLength(bodyLength);
    encodeHeaderActionCode(ac);
    encodeBody(str);
}

void SocketInfo::encode(const ull &token, const uint &bodyLength, const ActionCode &ac, const ptree &pt) {
    encode(token, bodyLength, ac, encodePtree(pt));
}

std::string SocketInfo::encodePtree(const ptree &pt, bool pretty) {
    std::stringstream ss;
    boost::property_tree::write_json(ss, pt, pretty);
    return std::move(ss.str());
}

void SocketInfo::decodePtree(const string &str, ptree &pt) {
    std::stringstream ss;
    ss << str;
    boost::property_tree::read_json(ss, pt);
}

const char *SocketInfo::getBuffer() const {
    return buffer;
}

char *SocketInfo::getBuffer() {
    return buffer;
}

