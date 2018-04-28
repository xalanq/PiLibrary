#include "socketwrapper.h"

SocketWrapper::SocketWrapper(boost::asio::ip::tcp::socket socket, SessionManager &sessionManager, UserManager &userManager) :
    socket(std::move(socket)),
    info(),
    sessionManager(sessionManager),
    userManager(userManager) {

}

void SocketWrapper::start() {
    cerr << "connected from " << socket.remote_endpoint().address() << '\n';
    doFilter();
}

void SocketWrapper::stop() {
    cerr << "closing socket from " << socket.remote_endpoint().address() << '\n';
    socket.close();
}

void SocketWrapper::doFilter() {
    info.setSize(SocketInfo::HEADER_SIZE);
    auto self(shared_from_this());
    boost::asio::async_read(
        socket,
        boost::asio::buffer(info.getBuffer(), SocketInfo::HEADER_SIZE),
        [this, self](const error_code &ec, size_t bytes) -> size_t {
            if (ec || (bytes > 0 && info.getBuffer()[bytes - 1] == SocketInfo::IDENTIFIER))
                return 0;
            return 1;
        },
        [this, self](const error_code &ec, size_t bytes) {
            cerr << "(doFilter), bytes: " << bytes << "\n";
            if (ec) {
                cerr << "(doFilter) Error from " << socket.remote_endpoint().address() << " : " << system_error(ec).what() << '\n';
                stop();
                return;
            }
            cerr << "(doFilter) bytes:" << bytes << '\n';
            if (bytes > 0 && info.getBuffer()[bytes - 1] == SocketInfo::IDENTIFIER) {
                doHeader();
                return;
            }
            doFilter();
        }
    );
}

void SocketWrapper::doHeader() {
    info.setSize(SocketInfo::HEADER_SIZE);
    auto self(shared_from_this());
    boost::asio::async_read(
        socket,
        boost::asio::buffer(info.getBuffer(), SocketInfo::HEADER_SIZE),
        boost::asio::transfer_exactly(SocketInfo::HEADER_SIZE),
        [this, self](const error_code &ec, size_t bytes) {
            cerr << "(doHeader)\n";
            if (ec) {
                cerr << "(doHeader) Error from " << socket.remote_endpoint().address() << " : " << system_error(ec).what() << '\n';
                stop();
                return;
            }
            X::ull token = info.decodeHeaderToken();
            X::uint length = info.decodeHeaderLength();
            cerr << "bytes: " << bytes << '\n';
            for (int i = 0; i < SocketInfo::HEADER_SIZE; ++i)
                cerr << int(info.getBuffer()[i]) << ' ';
            cerr << '\n';
            cerr << "token: " << token << ", length: " << length << '\n';
            if (length > SocketInfo::BODY_SIZE) {
                cerr << "(doHeader) Error from " << socket.remote_endpoint().address() << " : body size is too big.\n";
                doFilter();
                return;
            }
            doBody(token, length);
        }
    );
}

void SocketWrapper::doBody(X::ull token, X::uint length) {
    info.setSize(SocketInfo::BODY_SIZE);
    auto self(shared_from_this());
    boost::asio::async_read(
        socket,
        boost::asio::buffer(info.getBuffer(), length),
        boost::asio::transfer_exactly(length),
        [this, self, token, length](const error_code &ec, size_t bytes) {
            cerr << "(doBody)\n";
            if (ec) {
                cerr << "(doBody) Error from " << socket.remote_endpoint().address() << " : " << system_error(ec).what() << '\n';
                stop();
                return;
            }
            if (length != bytes) {
                cerr << "(doBody) Error from " << socket.remote_endpoint().address() << " : length != bytes\n";
                doFilter();
                return;
            }

            boost::property_tree::ptree pt;

            try {
                info.decodeBody(length, pt);
            } catch (std::exception &e) {
                cerr << "(doBody) Error from " << socket.remote_endpoint().address() << " : " << e.what() << '\n';
                doFilter();
                return;
            }

            auto p = sessionManager.findToken(token);
            if (p != nullptr) {
                doMain(std::move(pt), token, p->getUserid());
            } else { // login
                X::string username = pt.get("username", "");
                X::string password = pt.get("password", "");
                int priority = pt.get<int>("priority", 0);
                cerr << "(doBody) Login from " << socket.remote_endpoint().address() << " : [" << username << ", " << password << ", " << priority << "]\n";
                userManager.registerUser(username, password, priority);
                doFilter();
            }
        }
    );
}

void SocketWrapper::doMain(boost::property_tree::ptree pt, X::ull token, X::uint userid) {

}

