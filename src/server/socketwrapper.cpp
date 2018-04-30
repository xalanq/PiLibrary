#include "socketwrapper.h"

#define _from(func) cerr << "(" << #func << ") from " << socket.remote_endpoint().address() << " : " 
#define _to(func) cerr << "(" << #func << ") send to " << socket.remote_endpoint().address() << " : " 

SocketWrapper::SocketWrapper(boost::asio::ip::tcp::socket socket, SessionManager &sessionManager, UserManager &userManager) :
    socket(std::move(socket)),
    info(),
    sessionManager(sessionManager),
    userManager(userManager) {

}

void SocketWrapper::start() {
    _from(start) << "connected\n";
    read();
}

void SocketWrapper::stop() {
    _from(stop) << "stop\n";
    socket.close();
}

void SocketWrapper::read() {
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
            if (ec) {
                _from(read) << system_error(ec).what() << '\n';
                stop();
                return;
            }
            if (bytes > 0 && info.getBuffer()[bytes - 1] == SocketInfo::IDENTIFIER)
                readHeader();
            else
                read();
        }
    );
}

void SocketWrapper::readHeader() {
    info.setSize(SocketInfo::HEADER_SIZE);
    auto self(shared_from_this());
    boost::asio::async_read(
        socket,
        boost::asio::buffer(info.getBuffer(), SocketInfo::HEADER_SIZE),
        boost::asio::transfer_exactly(SocketInfo::HEADER_SIZE),
        [this, self](const error_code &ec, size_t bytes) {
            if (ec) {
                _from(readHeader) << system_error(ec).what() << '\n';
                stop();
                return;
            }
            auto token = info.decodeHeaderToken();
            auto length = info.decodeHeaderLength();
            auto ac = info.decodeHeaderActionCode();
            _from(readHeader) << "token: " << token << ", length: " << length << ", action_code: " << X::what(ac) << '\n';
            if (length > SocketInfo::BODY_SIZE) {
                _from(readHeader) << "body size is too big.\n";
                read();
                return;
            }
            readBody(std::move(token), std::move(length), std::move(ac));
        }
    );
}

void SocketWrapper::readBody(ull token, uint length, ActionCode ac) {
    info.setSize(length);
    auto self(shared_from_this());
    boost::asio::async_read(
        socket,
        boost::asio::buffer(info.getBuffer(), length),
        boost::asio::transfer_exactly(length),
        [this, self, token, ac](const error_code &ec, size_t bytes) {
            if (ec) {
                _from(readBody) << system_error(ec).what() << '\n';
                stop();
                return;
            }
            ptree pt;
            try {
                info.decodeBody(bytes, pt);
            } catch (std::exception &e) {
                _from(readBody) << "can't decode body. " << e.what() << '\n';
                read();
                return;
            }
            if (ac == X::Login)
                doLogin(pt, token);
            else if (ac == X::Register)
                doRegister(pt, token);
            else if (ac == X::Logout)
                doLogout(pt, token);
        }
    );
}

void SocketWrapper::write(const ull &token, const ptree &pt, const ActionCode &ac) {
    auto self(shared_from_this());
    string str = SocketInfo::encodePtree(pt);
    auto size = SocketInfo::HEADER_SIZE + 1 + str.size();
    info.setSize(size);
    info.encode(token, str.size(), X::LoginFeedback, str);
    _to(write) << "sending feedback, size = " << size << '\n';

    boost::asio::async_write(
        socket,
        boost::asio::buffer(info.getBuffer(), size),
        [this, self](const error_code &ec, size_t bytes) {
            if (ec) {
                _to(write) << system_error(ec).what() << '\n';
                stop();
                return;
            }
            _to(write) << "succeed to send\n";
            read();
        }
    );
}

void SocketWrapper::doLogin(const ptree &pt, const ull &token) {
    if (token != 0) {
        _from(doLogin) << "token != 0\n";
        writeLogin(0, ptree(), X::LoginFailed);
        return;
    }
    string username = pt.get("username", "");
    string password = pt.get("password", "");
    _from(doLogin) << "username: " << username << ", password: " << password << '\n';
    auto info = userManager.loginUser(username, password);
    if (info.empty()) {
        _from(doLogin) << "info is empty\n";
        writeLogin(0, std::move(info), X::NoSuchUser);
    } else {
        auto token = sessionManager.getRandToken();
        auto userid = info.get<uint> ("userid", 0);
        _from(doLogin) << "token: " << token << ", userid: " << userid << '\n';
        if (sessionManager.add(token, userid, Session::getNowTime() + sessionManager.getDefaultAlive())) {
            _from(doLogin) << "login successfully\n";
            writeLogin(token, std::move(info), X::NoError);
        } else {
            _from(doLogin) << "failed to add a new session, already exist\n";
            writeLogin(0, ptree(), X::AlreadyLogin);
        }
    }
}

void SocketWrapper::writeLogin(const ull &token, ptree pt, ErrorCode ec) {
    pt.put("error_code", ec);
    write(token, pt, X::LoginFeedback);
}

void SocketWrapper::doRegister(const ptree &pt, const ull &token) {
    if (token != 0) {
        _from(doRegister) << "token != 0\n";
        writeRegister(X::RegisterFailed);
        return;
    }
    string username = pt.get("username", "");
    string nickname = pt.get("nickname", "");
    string password = pt.get("password", "");
    string email = pt.get("email", "");
    _from(doRegister) << "username: " << username << ", nickname: " << nickname << ", password: " << password << ", email: " << email << '\n';

    auto ec = userManager.registerUser(username, nickname, password, email);
    if (!ec)
        _from(doRegister) << "success to register\n";
    else
        _from(doRegister) << "fail to register: " << X::what(ec) << '\n';
    writeRegister(ec);
}

void SocketWrapper::writeRegister(ErrorCode ec) {
    ptree pt;
    pt.put("error_code", ec);
    write(0, pt, X::RegisterFeedback);
}

void SocketWrapper::doLogout(const ptree &pt, const ull &token) {
    auto ec = X::NoError;
    if (token == 0) {
        _from(doLogout) << "token = 0\n";
        ec = X::NotLogin;
    } else if (!sessionManager.removeByToken(token)) {
        _from(doLogout) << "token is invalid\n";
        ec = X::InvalidToken;
    }
    writeLogout(ec);
}

void SocketWrapper::writeLogout(ErrorCode ec) {
    ptree pt;
    pt.put("error_code", ec);
    write(0, pt, X::LogoutFeedback);
}
