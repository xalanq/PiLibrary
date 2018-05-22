// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <ctime>
#include <sstream>

#include <boost/property_tree/json_parser.hpp>

#include <core/SocketInfo.h>

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
        this->size = size;
    }
}

size_t SocketInfo::getSize() const {
    return size;
}

SocketInfo::xll SocketInfo::decodeHeaderToken() const {
    return *static_cast<const xll *> ((const void *)(buffer));
}

SocketInfo::xint SocketInfo::decodeHeaderLength() const {
    return *static_cast<const xint *> ((const void *)(buffer + sizeof(xll)));
}

SocketInfo::ActionCode SocketInfo::decodeHeaderActionCode() const {
    return *static_cast<const ActionCode *> ((const void *)(buffer + sizeof(xll) + sizeof(xint)));
}

void SocketInfo::decodeBody(const xint &length, ptree &pt) const {
    std::stringstream ss;
    const xstring str(buffer, length);
    ss << str;
    boost::property_tree::read_json(ss, pt);
}

void SocketInfo::encodeIndentifier() {
    buffer[0] = IDENTIFIER;
}

void SocketInfo::encodeHeaderToken(const xll &token) {
    memcpy(buffer + 1, (const void *)(&token), sizeof(xll));
}

void SocketInfo::encodeHeaderLength(const xint &length) {
    memcpy(buffer + 1 + sizeof(xll), (const void *)(&length), sizeof(xint));
}

void SocketInfo::encodeHeaderActionCode(const ActionCode &ac) {
    memcpy(buffer + 1 + sizeof(xll) + sizeof(xint), (const void *)(&ac), sizeof(ActionCode));
}

void SocketInfo::encodeBody(const char *data, const size_t &length) {
    memcpy(buffer + 1 + sizeof(xll) + sizeof(xint) + sizeof(ActionCode), data, length);
}

void SocketInfo::encodeFile(const xint &mainLength, const char *data, const size_t &length) {
    memcpy(buffer + mainLength, data, length);
}

void SocketInfo::encodeMain(const xll &token, const xint &bodyLength, const ActionCode &ac, const char *data, const size_t &length) {
    encodeIndentifier();
    encodeHeaderToken(token);
    encodeHeaderLength(bodyLength);
    encodeHeaderActionCode(ac);
    encodeBody(data, length);
}

void SocketInfo::encodeMain(const xll &token, const xint &bodyLength, const ActionCode &ac, const xstring &str) {
    encodeMain(token, bodyLength, ac, str.c_str(), str.size());
}

void SocketInfo::encodeMain(const xll &token, const xint &bodyLength, const ActionCode &ac, const ptree &pt) {
    encodeMain(token, bodyLength, ac, encodePtree(pt));
}

SocketInfo::xstring SocketInfo::encodePtree(const ptree &pt, bool pretty) {
    std::stringstream ss;
    boost::property_tree::write_json(ss, pt, pretty);
    return std::move(ss.str());
}

void SocketInfo::decodePtree(const xstring &str, ptree &pt) {
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

void SocketInfo::setBuffer(char *buffer) {
    this->buffer = buffer;
}
