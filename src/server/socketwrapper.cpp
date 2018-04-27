#include "socketwrapper.h"

SocketWrapper::SocketWrapper(boost::asio::ip::tcp::socket socket, SessionManager &sessionManager, UserManager &userManager) :
    socket(std::move(socket)),
    info(),
    sessionManager(sessionManager),
    userManager(userManager) {

}

void SocketWrapper::start() {
    doFilter();
}

void SocketWrapper::stop() {
    socket.close();
}

void SocketWrapper::doFilter() {
    info.setMatchIndentifierPosition(0);
    auto self(shared_from_this());
    boost::asio::async_read(
        socket,
        boost::asio::buffer(info.getBuffer(), SocketInfo::HEADER_SIZE),
        [this, self](const error_code &ec, size_t bytes) -> size_t {
            if (info.matchIdentifier(bytes))
                return 0;
            info.setMatchIndentifierPosition(bytes);
            return 1;
        },
        [this, self](const error_code &ec, size_t bytes) {
            if (ec) {
                cerr << "(doFilter) Error from " << socket.remote_endpoint().address() << " : " << system_error(ec).what() << '\n';
                stop();
                return;
            }
            if (bytes >= SocketInfo::IDENTIFIER_SIZE && info.matchIdentifier(bytes)) {
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
            if (ec) {
                cerr << "(doHeader) Error from " << socket.remote_endpoint().address() << " : " << system_error(ec).what() << '\n';
                stop();
                return;
            }
            X::ull token = info.decodeHeaderToken();
            X::uint length = info.decodeHeaderLength();
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
            }
        }
    );
}

void SocketWrapper::doMain(boost::property_tree::ptree pt, X::ull token, X::uint userid) {

}

