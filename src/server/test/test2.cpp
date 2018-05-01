#include "../xserver.h"
#include "../../core/socketinfo.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;
boost::asio::io_service io_service;

// for get book and set book
int main() {
    srand(time(0) + clock());
    try {
        tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 2333);
        boost::property_tree::ptree pt;
        SocketInfo info;
        X::string str;
        size_t size = 0;
        tcp::socket s(io_service);
        cerr << "connect to " << ep.address() << ":" << 2333 << '\n';
        s.connect(ep);

        // login
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
        // end login

        for (int op; std::cin >> op; ) {
            if (op == 1) { // get
                pt = boost::property_tree::ptree();
                pt.put("bookid", int(rand() % 5 + 1));
                str = SocketInfo::encodePtree(pt);
                cerr << "str: " << str << '\n';

                size = SocketInfo::HEADER_SIZE + 1 + str.size();
                info.setSize(size);
                info.encode(token, static_cast<X::uint> (str.size()), X::GetBook, str);

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
            } else if (op == 2 || op == 3) {
                pt = boost::property_tree::ptree();
                int t = rand() % 10 + 1;
                pt.put("bookid", int(op == 2 ? 0 : t));
                pt.put("title", (op == 2 ? "book " : "update book ") + std::to_string(t));
                pt.put("author", "xalanq");
                pt.put("ISBN", "ISBN??");
                pt.put("publisher", "me");
                pt.put("amount", int(233));
                pt.put("introduction", "this is a book");
                pt.put("priority", int(1));
                str = SocketInfo::encodePtree(pt);
                cerr << "str: " << SocketInfo::encodePtree(pt, true) << '\n';

                size = SocketInfo::HEADER_SIZE + 1 + str.size();
                info.setSize(size);
                info.encode(token, static_cast<X::uint> (str.size()), X::SetBook, str);

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
            } else {
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
                s.close();
            }
        }
    } catch (std::exception &e) {
        cerr << e.what() << '\n';
    }
    return 0;
}