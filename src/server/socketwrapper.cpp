// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <boost/property_tree/json_parser.hpp>

#include <server/socketwrapper.h>

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
            auto p = sessionManager.findToken(token);
            if ((p == nullptr || p->getPriority() < AbstractUser::ADMINISTER) && bytes > SocketInfo::BODY_SIZE) {
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
                info.decodeBody(static_cast<uint>(bytes), pt);
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
            else if (ac == X::GetBook)
                doGetBook(pt, token);
            else if (ac == X::SetBook)
                doSetBook(pt, token);
        }
    );
}

void SocketWrapper::write(const ull &token, const ptree &pt, const ActionCode &ac) {
    auto self(shared_from_this());
    string str = SocketInfo::encodePtree(pt);
    auto size = SocketInfo::HEADER_SIZE + 1 + str.size();
    info.setSize(size);
    info.encode(token, static_cast<uint>(str.size()), X::LoginFeedback, str);
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
    auto tr = ptree();
    auto ec = X::NoError;
    ull tk = 0;
    if (token != 0) {
        _from(doLogin) << "token != 0\n";
        ec = X::LoginFailed;
    } else {
        _from(doLogin);
        auto p = userManager.loginUser(pt);
        if (p.empty()) {
            _from(doLogin) << "p is empty\n";
            ec = X::NoSuchUser;
        } else {
            tk = sessionManager.getRandToken();
            auto userid = p.get<uint>("userid", 0);
            auto priority = p.get<uint>("priority", 0);
            _from(doLogin) << "token: " << tk << ", userid: " << userid << '\n';
            if (sessionManager.add(tk, userid, Session::getNowTime() + sessionManager.getDefaultAlive(), priority, true)) {
                _from(doLogin) << "login successfully\n";
                tr = std::move(p);
            } else {
                _from(doLogin) << "failed to login\n";
                ec = X::LoginFailed;
            }
        }
    }
    writeLogin(tk, std::move(tr), ec);
}

void SocketWrapper::writeLogin(const ull &token, ptree pt, const ErrorCode &ec) {
    pt.put("error_code", ec);
    write(token, pt, X::LoginFeedback);
}

void SocketWrapper::doRegister(const ptree &pt, const ull &token) {
    if (token != 0) {
        _from(doRegister) << "token != 0\n";
        writeRegister(X::RegisterFailed);
        return;
    }
    _from(doRegister);
    auto ec = userManager.registerUser(pt);
    if (!ec)
        _from(doRegister) << "succeed to register\n";
    else
        _from(doRegister) << "fail to register: " << X::what(ec) << '\n';
    writeRegister(ec);
}

void SocketWrapper::writeRegister(const ErrorCode &ec) {
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

void SocketWrapper::writeLogout(const ErrorCode &ec) {
    ptree pt;
    pt.put("error_code", ec);
    write(0, pt, X::LogoutFeedback);
}

void SocketWrapper::doGetBook(const ptree &pt, const ull &token) {
    auto tr = ptree();
    auto ec = X::NoError;
    ull tk = 0;
    if (token == 0) {
        _from(doGetBook) << "token == 0\n";
        ec = X::NotLogin;
    } else {
        auto it = sessionManager.findToken(token);
        if (it == nullptr) {
            _from(doGetBook) << "not found session\n";
            ec = X::NotLogin;
        } else {
            tk = token;
            _from(doGetBook);
            auto p = userManager.getBookCore(pt);
            if (p.empty()) {
                _from(doGetBook) << "p is empty\n";
                ec = X::InvalidBook;
            } else {
                auto bookPriority = p.get<uint>("priority", AbstractUser::SUPER_ADMINISTER);
                auto priority = it->getPriority();
                _from(doGetBook) << "userid: " << it->getUserid() << ", priority: " << priority << ", bookPriority: " << bookPriority << '\n';
                if (bookPriority > priority)
                    ec = X::NoPermission;
                else
                    tr = std::move(p);
            }
        }
    }
    writeGetBook(tk, std::move(tr), ec);
}

void SocketWrapper::writeGetBook(const ull &token, ptree pt, const ErrorCode &ec) {
    pt.put("error_code", ec);
    write(token, pt, X::GetBookFeedback);
}

void SocketWrapper::doSetBook(const ptree &pt, const ull &token) {
    auto ec = X::NoError;
    ull tk = 0;
    if (token == 0) {
        _from(doSetBook) << "token = 0\n";
        ec = X::NotLogin;
    } else {
        auto it = sessionManager.findToken(token);
        if (it == nullptr) {
            _from(doSetBook) << "not found session\n";
            ec = X::NotLogin;
        } else {
            tk = token;
            uint bookPriority = pt.get<uint>("priority", AbstractUser::SUPER_ADMINISTER);
            auto priority = it->getPriority();
            _from(doSetBook) << "userid: " << it->getUserid() << ", priority: " << priority << ", bookPriority: " << bookPriority << '\n';
            if (bookPriority > priority || priority < AbstractUser::ADMINISTER) {
                _from(doSetBook) << "no permission\n";
                ec = X::NoPermission;
            } else {
                _from(doSetBook);
                ec = userManager.setBookCore(pt);
                if (!ec)
                    _from(doSetBook) << "succeed to set a book\n";
                else
                    _from(doSetBook) << "fail to set a book: " << X::what(ec) << '\n';
            }
        }
    }
    writeSetBook(tk, ec);
}

void SocketWrapper::writeSetBook(const ull &token, const ErrorCode &ec) {
    ptree pt;
    pt.put("error_code", ec);
    write(token, pt, X::SetBookFeedback);
}
