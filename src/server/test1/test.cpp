#include "../xserver.h"
#include "../../core/socketinfo.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;
boost::asio::io_service io_service;

int main() {
    try {
        tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 2333);
        tcp::socket s(io_service);
        cerr << "connect to " << ep.address() << '\n';
        s.connect(ep);

        boost::property_tree::ptree pt;
        pt.put("username", "xalanq");
        pt.put("password", "hash(123456)");
        pt.put<int>("priority", 1);
        X::string str = SocketInfo::encodePtree(pt);
        cerr << "str: " << str << '\n';

        SocketInfo info;
        info.setSize(1 + SocketInfo::HEADER_SIZE);
        info.encodeHeader(0, str.size());
        for (int i = 0; i < SocketInfo::HEADER_SIZE + 1; ++i)
            cerr << int(info.getBuffer()[i]) << ' ';
        cerr << '\n';

        cerr << "sending header\n";
        boost::asio::write(
            s,
            boost::asio::buffer(info.getBuffer(), 1 + SocketInfo::HEADER_SIZE)
        );

        cerr << "sending body\n";
        info.setSize(SocketInfo::BODY_SIZE);
        info.encodeBody(str);
        boost::asio::write(
            s,
            boost::asio::buffer(info.getBuffer(), str.size())
        );

        cerr << "complete\n";
        s.close();
    } catch(std::exception &e) {
        cerr << e.what() << '\n';
    }

    return 0;
}