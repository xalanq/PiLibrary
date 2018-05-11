// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <core/xcore.h>
#include <core/socketinfo.h>

using boost::asio::ip::tcp;
boost::asio::io_service io_service;
tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 2333);

// for login, logout, register
int main() {
    srand(time(0) + clock());
    try {
        for (int op; std::cin >> op; ) {
            tcp::socket s(io_service);
            X::xll token = 0;
            boost::property_tree::ptree pt;
            X::ActionCode ac = X::NoAction;

            cerr << "connect to " << ep.address() << ":" << 2333 << '\n';
            s.connect(ep);

            // login
            pt.put("username", "xalanq" + std::to_string(op));
            pt.put("password", "hash(123456)");
            cerr << "Login send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

            X::tcp_sync_write(s, 0, X::Login, pt);
            pt = boost::property_tree::ptree();
            X::tcp_sync_read(s, token, ac, pt);

            cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';

            // GetLoginRecord
            pt = boost::property_tree::ptree();
            pt.put("number", 50);
            pt.put("begin", 0);
            cerr << "GetLoginRecord send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

            X::tcp_sync_write(s, token, X::GetLoginRecord, pt);
            pt = boost::property_tree::ptree();
            X::tcp_sync_read(s, token, ac, pt);

            cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';

            // GetBorrowRecord
            pt = boost::property_tree::ptree();
            pt.put("number", 50);
            pt.put("begin", 0);
            cerr << "GetBorrowRecord send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

            X::tcp_sync_write(s, token, X::GetBorrowRecord, pt);
            pt = boost::property_tree::ptree();
            X::tcp_sync_read(s, token, ac, pt);

            cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';

            // GetKeep
            pt = boost::property_tree::ptree();
            pt.put("number", 50);
            pt.put("begin", 0);
            cerr << "GetKeep send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

            X::tcp_sync_write(s, token, X::GetKeepRecord, pt);
            pt = boost::property_tree::ptree();
            X::tcp_sync_read(s, token, ac, pt);

            cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';

            // GetBrowseRecord
            pt = boost::property_tree::ptree();
            pt.put("number", 50);
            pt.put("begin", 0);
            cerr << "GetBrowseRecord send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

            X::tcp_sync_write(s, token, X::GetBrowseRecord, pt);
            pt = boost::property_tree::ptree();
            X::tcp_sync_read(s, token, ac, pt);

            cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';

            // logout
            pt = boost::property_tree::ptree();
            cerr << "Logout send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

            X::tcp_sync_write(s, token, X::Logout, pt);
            pt = boost::property_tree::ptree();
            X::tcp_sync_read(s, token, ac, pt);

            cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
            s.close();
        }
    } catch (std::exception &e) {
        cerr << e.what() << '\n';
    }

    return 0;
}