// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <core/types.h>

class SocketInfo {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;
    typedef X::ActionCode ActionCode;

    enum {
        HEADER_SIZE = sizeof(xll) + sizeof(xint) + sizeof(ActionCode), 
        BODY_SIZE = 2048
    };
    static const char IDENTIFIER;

    SocketInfo();
    ~SocketInfo();
    SocketInfo(const SocketInfo &) = delete;
    SocketInfo &operator = (const SocketInfo &) = delete;

    void setSize(const size_t &size);
    size_t getSize() const;

    xll decodeHeaderToken() const;
    xint decodeHeaderLength() const;
    ActionCode decodeHeaderActionCode() const;
    void decodeBody(const xint &length, ptree &pt) const;

    void encodeIndentifier();
    void encodeHeaderToken(const xll &token);
    void encodeHeaderLength(const xint &length);
    void encodeHeaderActionCode(const ActionCode &ac);
    void encodeBody(const xstring &str);
    void encode(const xll &token, const xint &bodyLength, const ActionCode &ac, const xstring &str);
    void encode(const xll &token, const xint &bodyLength, const ActionCode &ac, const ptree &pt);

    static xstring encodePtree(const ptree &pt, bool pretty = false);
    static void decodePtree(const xstring &str, ptree &pt);

    const char *getBuffer() const;
    char *getBuffer();

private:
    char *buffer;
    size_t size;
};
