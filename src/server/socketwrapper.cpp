#include "socketwrapper.h"

#define _from(func) cerr << "(" << #func << ") from " << socket.remote_endpoint().address() << " : " 
#define _to(func) cerr << "(" << #func << ") send to " << socket.remote_endpoint().address() << " : " 

SocketWrapper::SocketWrapper(boost::asio::ip::tcp::socket socket, SessionManager &sessionManager, UserManager &userManager) :
    socket(std::move(socket)),
    infoIn(),
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
    infoIn.setSize(SocketInfo::HEADER_SIZE);
    auto self(shared_from_this());
    boost::asio::async_read(
        socket,
        boost::asio::buffer(infoIn.getBuffer(), SocketInfo::HEADER_SIZE),
        [this, self](const error_code &ec, size_t bytes) -> size_t {
            if (ec || (bytes > 0 && infoIn.getBuffer()[bytes - 1] == SocketInfo::IDENTIFIER))
                return 0;
            return 1;
        },
        [this, self](const error_code &ec, size_t bytes) {
            if (ec) {
                _from(read) << system_error(ec).what() << '\n';
                stop();
                return;
            }
            if (bytes > 0 && infoIn.getBuffer()[bytes - 1] == SocketInfo::IDENTIFIER)
                readHeader();
            else
                read();
        }
    );
}

void SocketWrapper::readHeader() {
    infoIn.setSize(SocketInfo::HEADER_SIZE);
    auto self(shared_from_this());
    boost::asio::async_read(
        socket,
        boost::asio::buffer(infoIn.getBuffer(), SocketInfo::HEADER_SIZE),
        boost::asio::transfer_exactly(SocketInfo::HEADER_SIZE),
        [this, self](const error_code &ec, size_t bytes) {
            if (ec) {
                _from(readHeader) << system_error(ec).what() << '\n';
                stop();
                return;
            }
            auto token = infoIn.decodeHeaderToken();
            auto length = infoIn.decodeHeaderLength();
            auto ac = infoIn.decodeHeaderActionCode();
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
    infoIn.setSize(length);
    auto self(shared_from_this());
    boost::asio::async_read(
        socket,
        boost::asio::buffer(infoIn.getBuffer(), length),
        boost::asio::transfer_exactly(length),
        [this, self, token, ac](const error_code &ec, size_t bytes) {
            if (ec) {
                _from(readBody) << system_error(ec).what() << '\n';
                stop();
                return;
            }
            ptree pt;
            try {
                infoIn.decodeBody(bytes, pt);
            } catch (std::exception &e) {
                _from(readBody) << "can't decode body. " << e.what() << '\n';
                read();
                return;
            }
            if (ac == X::Login)
                doLogin(pt, token);
            else if (ac == X::Register)
                doRegister(pt, token);
            read();
        }
    );
}

void SocketWrapper::write(const ull &token, const ptree &pt, const ActionCode &ac) {
    auto self(shared_from_this());
    string str = SocketInfo::encodePtree(pt);
    auto size = SocketInfo::HEADER_SIZE + 1 + str.size();
    auto info = std::make_shared<SocketInfo>();
    info->setSize(size);
    info->encode(token, str.size(), X::LoginFeedback, str);
    _to(write) << "sending feedback, size = " << size << '\n';

    boost::asio::async_write(
        socket,
        boost::asio::buffer(info->getBuffer(), size),
        [this, self, info](const error_code &ec, size_t bytes) {
            if (ec) {
                _to(write) << system_error(ec).what() << '\n';
                stop();
                return;
            }
            _to(write) << "succeed to send\n";
        }
    );
}

void SocketWrapper::doLogin(const ptree &pt, const ull &token) {
    if (token != 0) {
        _from(doLogin) << "token != 0\n";
        writeLogin(0, X::LoginFailed);
        return;
    }
    string username = pt.get("username", "");
    string password = pt.get("password", "");
    _from(doLogin) << "username: " << username << ", password: " << password << '\n';
    auto userid = userManager.loginUser(username, password);
    _from(doLogin) << "userid: " << userid << '\n';
    if (userid == 0) {
        writeLogin(0, X::NoSuchUser);
    } else {
        auto token = sessionManager.getRandToken();
        _from(doLogin) << "token: " << token << '\n';
        if (sessionManager.add(token, userid, Session::getNowTime() + sessionManager.getDefaultAlive())) {
            writeLogin(token);
        } else {
            _from(doLogin) << "failed to add a new session, already exist\n";
            writeLogin(0, X::AlreadyLogin);
        }
    }
}

void SocketWrapper::writeLogin(const ull &token, ErrorCode ec) {
    ptree pt;
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
    string password = pt.get("password", "");
    string email = pt.get("email", "");
    _from(doRegister) << "username: " << username << ", password: " << password << ", email: " << email << '\n';

    auto ec = userManager.registerUser(username, password, email);
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
