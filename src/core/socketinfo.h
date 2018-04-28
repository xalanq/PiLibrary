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
        HEADER_SIZE = sizeof(X::ull) + sizeof(X::uint), 
        BODY_SIZE = 1024, 
        PACKET_SIZE = 1024 * 24
    };
    static const char IDENTIFIER;

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

    static X::string encodePtree(const boost::property_tree::ptree &pt);
    void encodeBody(const X::string &str);
    void decodeBody(const X::uint &length, boost::property_tree::ptree &pt) const;

    const char *getBuffer() const;
    char *getBuffer();

private:
    char *buffer;
    int size;
};

#endif // SOCKETINFO_H
