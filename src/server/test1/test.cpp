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
        X::string str = SocketInfo::encodePtree(pt);
        cerr << "str: " << str << '\n';

        auto size = SocketInfo::HEADER_SIZE + 1 + str.size();

        SocketInfo info;
        info.setSize(size);
        info.encode(0, str.size(), X::Login, str);
        /*
        for (auto i = 0; i < size; ++i)
            cerr << int(info.getBuffer()[i]) << ' ';
        cerr << '\n';
        */

        /*
        cerr << "sending\n";
        boost::asio::write(
            s,
            boost::asio::buffer(info.getBuffer(), size)
        );
        cerr << "complete\n";
        */

        // pt.put("username", "xalanq");
        srand(time(0));
        pt.put("username", "xalanq" + std::to_string(rand() % 10));
        pt.put("password", "hash(123456)");
        pt.put("email", "xalanq@gmail.com");
        str = SocketInfo::encodePtree(pt);
        cerr << "str: " << str << '\n';

        size = SocketInfo::HEADER_SIZE + 1 + str.size();

        info.setSize(size);
        info.encode(0, str.size(), X::Register, str);
        /*
        for (auto i = 0; i < size; ++i)
            cerr << int(info.getBuffer()[i]) << ' ';
        cerr << '\n';
        */

        cerr << "sending\n";
        boost::asio::write(
            s,
            boost::asio::buffer(info.getBuffer(), size)
        );
        cerr << "complete\n";

        s.close();
    } catch(std::exception &e) {
        cerr << e.what() << '\n';
    }

    return 0;
}