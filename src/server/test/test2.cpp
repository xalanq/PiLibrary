// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <ctime>

#include <boost/asio.hpp>

#include <core/xcore.h>
#include <core/socketinfo.h>

using boost::asio::ip::tcp;
boost::asio::io_service io_service;
tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 2333);

void readme() {
    using std::cout;
    cout << "======================================\n";
    cout << "op:\n";
    cout << "1. Get Book\n";
    cout << "2. Add Book\n";
    cout << "3. Set Book\n";
    cout << "4. Borrow Book\n";
    cout << "5. Return Book\n";
    cout << "6. Star Book\n";
    cout << "7. Unstar Book\n";
    cout << "8. Get New Book List\n";
    cout << "Otherwise. Logout\n";
    cout << "input op: ";
}

// for book operations
int main() {
    srand(time(0) + clock());
    try {
        tcp::socket s(io_service);
        X::xll token = 0;
        boost::property_tree::ptree pt;
        X::ActionCode ac = X::NoAction;

        cerr << "connect to " << ep.address() << ":" << 2333 << '\n';
        s.connect(ep);
        
        // login
        int id;
        std::cout << "Login ID: ";
        std::cin >> id;
        pt.put("username", "xalanq" + std::to_string(id));
        pt.put("password", "hash(123456)");
        cerr << "Login send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

        X::tcp_sync_write(s, 0, X::Login, pt);
        pt = ptree();
        X::tcp_sync_read(s, token, ac, pt);

        cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
        X::xll loginToken = token;
        X::xint userid = pt.get<X::xint>("userid");

        int op;
        for (readme(); std::cin >> op; readme()) {
            token = loginToken;
            pt = ptree();
            if (op == 1) {
                // GetBook
                std::cout << "input bookid: ";
                int bookid;
                std::cin >> bookid;
                pt.put("bookid", bookid);
                cerr << "GetBook send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, token, X::GetBook, pt);
                pt = ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
            } else if (op == 2 || op == 3) {
                static int cnt = 0;
                // SetBook or ModifyBook
                int t;
                if (op == 2)
                    t = ++cnt;
                else {
                    std::cout << "input bookid: ";
                    std::cin >> t;
                }
                pt.put("bookid", int(op == 2 ? 0 : t));
                pt.put("title", (op == 2 ? "book " : "update book ") + std::to_string(t));
                pt.put("author", "xalanq");
                pt.put("ISBN", "ISBN??");
                pt.put("publisher", "me");
                pt.put("amount", 233);
                pt.put("introduction", "this is a book");
                pt.put("position", "li wen zheng");
                pt.put("priority", 1);
                cerr << "GetBook send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, token, X::SetBook, pt);
                pt = ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
            } else if (op == 4) {
                // BorrowBook
                std::cout << "input bookid: ";
                int bookid;
                std::cin >> bookid;
                pt.put("bookid", bookid);
                pt.put("beginTime", X::xll(time(0)));
                pt.put("endTime", X::xll(time(0)) + 1000000);
                cerr << "BorrowBook send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, token, X::BorrowBook, pt);
                pt = ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
            } else if (op == 5) {
                // ReturnBook
                std::cout << "input bookid: ";
                int bookid;
                std::cin >> bookid;
                pt.put("userid", userid);
                pt.put("bookid", bookid);
                cerr << "BorrowBook send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, token, X::ReturnBook, pt);
                pt = ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
            } else if (op == 6) {
                // StarBook
                std::cout << "input bookid: ";
                int bookid;
                std::cin >> bookid;
                pt.put("bookid", bookid);
                cerr << "StarBook send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, token, X::StarBook, pt);
                pt = ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
            } else if (op == 7) {
                // StarBook
                std::cout << "input bookid: ";
                int bookid;
                std::cin >> bookid;
                pt.put("bookid", bookid);
                cerr << "UnStarBook send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, token, X::UnStarBook, pt);
                pt = ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
            } else if (op == 8) {
                // GetNewBookList
                std::cout << "input number: ";
                int number;
                std::cin >> number;
                pt.put("number", number);
                cerr << "GetNewBookList send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, token, X::GetNewBookList, pt);
                pt = ptree();
                X::tcp_sync_read(s, token, ac, pt);

                cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
            } else {
                // logout
                pt = ptree();
                cerr << "Logout send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

                X::tcp_sync_write(s, token, X::Logout, pt);
                pt = ptree();
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