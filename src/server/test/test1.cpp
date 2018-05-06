#include "../xserver.h"
#include "../../core/socketinfo.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>

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
            X::ActionCode ac;

            cerr << "connect to " << ep.address() << ":" << 2333 << '\n';
            s.connect(ep);

            if (op == 1) {
                // login
                pt.put("username", "xalanq" + std::to_string(rand() % 10));
                pt.put("password", "hash(123456)");
                cerr << "Login send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, 0, X::Login, pt);
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
            } else {
                // register
                pt.put("username", "xalanq" + std::to_string(rand() % 10));
                pt.put("nickname", "Alan Clarke");
                pt.put("password", "hash(123456)");
                pt.put("email", "xalanq@gmail.com");
                cerr << "Register send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, 0, X::Register, pt);
                pt = boost::property_tree::ptree();
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