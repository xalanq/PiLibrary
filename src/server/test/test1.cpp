// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <boost/asio.hpp>

#include <core/SocketInfo.h>
#include <core/utils.h>

using boost::asio::ip::tcp;
boost::asio::io_service io_service;
tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 2333);

typedef X::xll xll;
typedef X::xint xint;
typedef X::ptree ptree;
typedef X::xstring xstring;
typedef X::ActionCode ActionCode;
typedef X::ErrorCode ErrorCode;

// for login, logout, register
int main() {
    srand(time(0) + clock());
    try {
        for (int op; std::cin >> op; ) {
            tcp::socket s(io_service);
            xll token = 0;
            ptree pt;
            ActionCode ac = X::NoAction;

            cerr << "connect to " << ep.address() << ":" << 2333 << '\n';
            s.connect(ep);

            if (op == 1) {
                // login
                pt.put("username", "xalanq" + std::to_string(rand() % 10));
                pt.put("password", "e472685f5f963edaaae04708972d6442e4b397e3");
                cerr << "Login send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, 0, X::Login, pt);
                pt = ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';


                // logout
                pt = ptree();
                cerr << "Logout send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, token, X::Logout, pt);
                pt = ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
            } else {
                // register
                pt.put("username", "xalanq" + std::to_string(rand() % 10));
                pt.put("nickname", "Alan Clarke");
                pt.put("password", "e472685f5f963edaaae04708972d6442e4b397e3");
                pt.put("email", "xalanq@gmail.com");
                cerr << "Register send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, 0, X::Register, pt);
                pt = ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
            }
            s.close();
        }
    } catch (std::exception &e) {
        cerr << e.what() << '\n';
    }

    return 0;
}