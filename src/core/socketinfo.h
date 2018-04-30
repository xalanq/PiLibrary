#ifndef SOCKETINFO_H
#define SOCKETINFO_H

#include "xcore.h"
#include <ctime>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class SocketInfo {
public:
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;
    typedef X::ActionCode ActionCode;
    typedef boost::property_tree::ptree ptree;

    enum {
        HEADER_SIZE = sizeof(ull) + sizeof(uint) + sizeof(ActionCode), 
        BODY_SIZE = 1024, 
        PACKET_SIZE = 1024 * 24
    };
    static const char IDENTIFIER;

    SocketInfo();
    ~SocketInfo();
    SocketInfo(const SocketInfo &) = delete;
    SocketInfo &operator = (const SocketInfo &) = delete;

    void setSize(const size_t &size);
    size_t getSize() const;

    ull decodeHeaderToken() const;
    uint decodeHeaderLength() const;
    ActionCode decodeHeaderActionCode() const;
    void decodeBody(const uint &length, ptree &pt) const;

    void encodeIndentifier();
    void encodeHeaderToken(const ull &token);
    void encodeHeaderLength(const uint &length);
    void encodeHeaderActionCode(const ActionCode &ac);
    void encodeBody(const string &str);
    void encode(const ull &token, const uint &bodyLength, const ActionCode &ac, const string &str);
    void encode(const ull &token, const uint &bodyLength, const ActionCode &ac, const ptree &pt);

    static std::string encodePtree(const ptree &pt);
    static void decodePtree(const string &str, ptree &pt);

    const char *getBuffer() const;
    char *getBuffer();

private:
    char *buffer;
    size_t size;
};

#endif // SOCKETINFO_H
