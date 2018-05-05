#include "../xserver.h"
#include "../../core/socketinfo.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;
boost::asio::io_service io_service;
tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 2333);

// for get book and set book
int main() {
    srand(time(0) + clock());
    try {
        tcp::socket s(io_service);
        X::ull token = 0;
        boost::property_tree::ptree pt;
        X::ActionCode ac;

        cerr << "connect to " << ep.address() << ":" << 2333 << '\n';
        s.connect(ep);
        
        // login
        pt.put("username", "xalanq" + std::to_string(rand() % 10));
        pt.put("password", "hash(123456)");
        cerr << "Login send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

        X::tcp_sync_write(s, 0, X::Login, pt);
        pt = boost::property_tree::ptree();
        X::tcp_sync_read(s, token, ac, pt);

        cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
        X::ull loginToken = token;

        for (int op; std::cin >> op; ) {
            token = loginToken;
            pt = boost::property_tree::ptree();
            if (op == 1) {
                // GetBook
                pt.put("bookid", int(rand() % 5 + 1));
                cerr << "GetBook send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, token, X::GetBook, pt);
                pt = boost::property_tree::ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
            } else if (op == 2 || op == 3) {
                static int cnt = 0;
                // SetBook or ModifyBook
                int t = ++cnt;
                pt.put("bookid", int(op == 2 ? 0 : t));
                pt.put("title", (op == 2 ? "book " : "update book ") + std::to_string(t));
                pt.put("author", "xalanq");
                pt.put("ISBN", "ISBN??");
                pt.put("publisher", "me");
                pt.put("amount", int(233));
                pt.put("introduction", "this is a book");
                pt.put("position", "li wen zheng");
                pt.put("priority", int(1));
                cerr << "GetBook send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, token, X::SetBook, pt);
                pt = boost::property_tree::ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
            } else {
                // logout
                pt = boost::property_tree::ptree();
                cerr << "Logout send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, token, X::Logout, pt);
                pt = boost::property_tree::ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
                s.close();
            }
        }
    } catch (std::exception &e) {
        cerr << e.what() << '\n';
    }
    return 0;
}