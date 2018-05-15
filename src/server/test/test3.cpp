// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <boost/asio.hpp>
#include <boost/thread.hpp>

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

// for get record
int main() {
    srand(time(0) + clock());
    try {
        for (int op; std::cin >> op; ) {
            tcp::socket s(io_service);
            xll token = 0;
            boost::property_tree::ptree pt;
            ActionCode ac = X::NoAction;

            cerr << "connect to " << ep.address() << ":" << 2333 << '\n';
            s.connect(ep);

            // login
            pt.put("username", "xalanq" + std::to_string(op));
            pt.put("password", "hash(123456)");
            cerr << "Login send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

            X::tcp_sync_write(s, 0, X::Login, pt);
            pt = ptree();
            X::tcp_sync_read(s, token, ac, pt);

            cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';

            // GetLoginRecord
            pt = ptree();
            pt.put("number", 50);
            pt.put("begin", 0);
            cerr << "GetLoginRecord send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

            X::tcp_sync_write(s, token, X::GetLoginRecord, pt);
            pt = ptree();
            X::tcp_sync_read(s, token, ac, pt);

            cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';

            // GetStarRecord
            pt = ptree();
            pt.put("number", 50);
            pt.put("begin", 0);
            cerr << "GetStarRecord send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

            X::tcp_sync_write(s, token, X::GetStarRecord, pt);
            pt = ptree();
            X::tcp_sync_read(s, token, ac, pt);

            cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';

            // GetBorrowRecord
            pt = ptree();
            pt.put("number", 50);
            pt.put("begin", 0);
            cerr << "GetBorrowRecord send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

            X::tcp_sync_write(s, token, X::GetBorrowRecord, pt);
            pt = ptree();
            X::tcp_sync_read(s, token, ac, pt);

            cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';

            // GetKeep
            pt = ptree();
            pt.put("number", 50);
            pt.put("begin", 0);
            cerr << "GetKeep send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

            X::tcp_sync_write(s, token, X::GetKeepRecord, pt);
            pt = ptree();
            X::tcp_sync_read(s, token, ac, pt);

            cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';

            // GetBrowseRecord
            pt = ptree();
            pt.put("number", 50);
            pt.put("begin", 0);
            cerr << "GetBrowseRecord send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

            X::tcp_sync_write(s, token, X::GetBrowseRecord, pt);
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
            s.close();
        }
    } catch (std::exception &e) {
        cerr << e.what() << '\n';
    }

    return 0;
}