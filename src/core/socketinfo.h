#ifndef SOCKETINFO_H
#define SOCKETINFO_H

#include "xcore.h"
#include <ctime>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class SocketInfo {
public:
    enum {
        IDENTIFIER_SIZE = 4, 
        HEADER_SIZE = IDENTIFIER_SIZE + sizeof(X::uint) + sizeof(X::ull), 
        BODY_SIZE = 1024, 
        PACKET_SIZE = 1024 * 24
    };
    static const char IDENTIFIER[IDENTIFIER_SIZE + 1];

    SocketInfo();
    ~SocketInfo();
    SocketInfo(const SocketInfo &) = delete;
    SocketInfo &operator = (const SocketInfo &) = delete;

    void setSize(const X::uint &size);
    int getSize() const;

    X::ull decodeHeaderToken() const;
    void encodeHeaderToken(const X::ull &token);

    X::uint decodeHeaderLength() const;
    void encodeHeaderLength(const X::uint &length);

    void encodeIndentifier();
    void encodeHeader(const X::ull &token, const X::uint &length);

    X::uint encodeBody(const boost::property_tree::ptree &pt);
    void decodeBody(const X::uint &length, boost::property_tree::ptree &pt) const;

    bool matchIdentifier(const size_t &length) const;
    void setMatchIndentifierPosition(const size_t &pos);

    const char *getBuffer() const;
    char *getBuffer();

private:
    char *buffer;
    int size;
    size_t matchIndentifierPosition;
};

#endif // SOCKETINFO_H
