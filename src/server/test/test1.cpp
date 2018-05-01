#include "../xserver.h"
#include "../../core/socketinfo.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;
boost::asio::io_service io_service;

// for login, logout, register
int main() {
    srand(time(0) + clock());
    try {
        tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 2333);
        for (int op; std::cin >> op; ) {
            boost::property_tree::ptree pt;
            SocketInfo info;
            X::string str;
            size_t size = 0;
            tcp::socket s(io_service);
            cerr << "connect to " << ep.address() << ":" << 2333 << '\n';
            s.connect(ep);
            if (op == 1) {
                pt.put("username", "xalanq" + std::to_string(rand() % 10));
                pt.put("password", "hash(123456)");
                str = SocketInfo::encodePtree(pt);
                cerr << "str: " << str << '\n';

                size = SocketInfo::HEADER_SIZE + 1 + str.size();
                info.setSize(size);
                info.encode(0, static_cast<X::uint> (str.size()), X::Login, str);

                cerr << "sending\n";
                boost::asio::write(
                    s,
                    boost::asio::buffer(info.getBuffer(), size)
                );
                cerr << "complete\n";

                boost::asio::read(
                    s,
                    boost::asio::buffer(info.getBuffer(), 1),
                    boost::asio::transfer_exactly(1)
                );
                boost::asio::read(
                    s,
                    boost::asio::buffer(info.getBuffer(), SocketInfo::HEADER_SIZE),
                    boost::asio::transfer_exactly(SocketInfo::HEADER_SIZE)
                );
                auto length = info.decodeHeaderLength();
                auto token = info.decodeHeaderToken();
                cerr << "length: " << length << ", token: " << token << '\n';

                info.setSize(length);
                boost::asio::read(
                    s,
                    boost::asio::buffer(info.getBuffer(), length),
                    boost::asio::transfer_exactly(length)
                );

                pt = boost::property_tree::ptree();
                info.decodeBody(length, pt);
                str = info.encodePtree(pt, true);
                cerr << str << '\n';

                pt = boost::property_tree::ptree();
                str = SocketInfo::encodePtree(pt);

                size = SocketInfo::HEADER_SIZE + 1 + str.size();
                info.setSize(size);
                info.encode(token, static_cast<X::uint> (str.size()), X::Logout, str);

                cerr << "sending logout\n";
                boost::asio::write(
                    s,
                    boost::asio::buffer(info.getBuffer(), size)
                );
                cerr << "complete\n";
            } else {
                pt.put("username", "xalanq" + std::to_string(rand() % 10));
                pt.put("nickname", "Alan Clarke");
                pt.put("password", "hash(123456)");
                pt.put("email", "xalanq@gmail.com");
                str = SocketInfo::encodePtree(pt);
                cerr << "str: " << str << '\n';

                size = SocketInfo::HEADER_SIZE + 1 + str.size();
                info.setSize(size);
                info.encode(0, static_cast<X::uint> (str.size()), X::Register, str);

                cerr << "sending\n";
                boost::asio::write(
                    s,
                    boost::asio::buffer(info.getBuffer(), size)
                );
                cerr << "complete\n";
            }
            s.close();
        }
    } catch (std::exception &e) {
        cerr << e.what() << '\n';
    }

    return 0;
}