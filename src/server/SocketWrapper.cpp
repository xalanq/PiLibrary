// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <boost/property_tree/json_parser.hpp>

#include <server/SocketWrapper.h>
#include <core/resource.h>
#include <core/utils.h>

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
            if ((p == nullptr || p->getPriority() < X::ADMINISTER) && bytes > SocketInfo::BODY_SIZE) {
                _from(readHeader) << "body size is too big.\n";
                write(X::UnknownError, X::Error);
            } else
                readBody(std::move(token), std::move(length), std::move(ac));
        }
    );
}

void SocketWrapper::readBody(const xll &token, const xint &length, const ActionCode &ac) {
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
                info.decodeBody(static_cast<xint>(bytes), pt);
            } catch (std::exception &e) {
                _from(readBody) << "can't decode body. " << e.what() << '\n';
                write(X::UnknownError, X::Error);
                return;
            }
            if (ac == X::Login)
                doLogin(std::move(pt), token);
            else if (ac == X::Register)
                doRegister(std::move(pt), token);
            else if (ac == X::Logout)
                doLogout(std::move(pt), token);
            else if (ac == X::Modify)
                doModify(std::move(pt), token);
            else if (ac == X::BorrowBook)
                doBorrowBook(std::move(pt), token);
            else if (ac == X::ReturnBook)
                doReturnBook(std::move(pt), token);
            else if (ac == X::StarBook)
                doStarBook(std::move(pt), token);
            else if (ac == X::UnStarBook)
                doUnStarBook(std::move(pt), token);
            else if (ac == X::GetBook)
                doGetBook(std::move(pt), token);
            else if (ac == X::GetBookBrief)
                doGetBookBrief(std::move(pt), token);
            else if (ac == X::SetBook)
                doSetBook(std::move(pt), token);
            else if (ac == X::GetLoginRecord)
                doGetRecord(std::move(pt), token, "loginRecord", X::GetLoginRecordFeedback);
            else if (ac == X::GetStarRecord)
                doGetRecord(std::move(pt), token, "starRecord", X::GetStarRecordFeedback);
            else if (ac == X::GetBorrowRecord)
                doGetRecord(std::move(pt), token, "borrowRecord", X::GetBorrowRecordFeedback);
            else if (ac == X::GetBrowseRecord)
                doGetRecord(std::move(pt), token, "browseRecord", X::GetBrowseRecordFeedback);
            else if (ac == X::GetKeepRecord)
                doGetRecord(std::move(pt), token, "keepRecord", X::GetKeepRecordFeedback);
            else if (ac == X::GetNewBookList)
                doGetNewBookList(std::move(pt), token);
            else if (ac == X::GetBookCover)
                doGetBookCover(std::move(pt), token);
            else if (ac == X::SetBookCover)
                doSetBookCover(std::move(pt), token);
            else
                write(X::UnknownError, X::Error);
        }
    );
}

void SocketWrapper::write(const ErrorCode &ec, const ActionCode &ac, const xll &token, ptree pt, const char *data, const size_t &dataSize) {
    pt.put("error_code", ec);
    if (dataSize)
        pt.put("fileSize", dataSize);

    auto self(shared_from_this());
    xstring str = SocketInfo::encodePtree(pt);
    auto bodySize = int(str.size());
    auto mainSize = SocketInfo::HEADER_SIZE + 1 + bodySize;
    auto size = mainSize + dataSize;

    info.setSize(size);
    info.encodeMain(token, bodySize, ac, str);
    if (dataSize)
        info.encodeFile(mainSize, data, dataSize);

    _to(write) << "sending data, size = " << size << '\n';

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

void SocketWrapper::saveFile(const ErrorCode &ec, const ActionCode &ac, const xll &token, const ptree &pt) {
    if (ec != X::NoError) {
        write(ec, ac, token);
        return;
    }
    size_t size = pt.get<size_t>("fileSize", 0);
    if (!size) {
        write(X::InvalidResource, ac, token);
        return;
    }

    auto self(shared_from_this());

    info.setSize(size);

    boost::asio::async_read(
        socket,
        boost::asio::buffer(info.getBuffer(), size),
        boost::asio::transfer_exactly(size),
        [this, self, ac, token, pt, size](const error_code &ec, size_t bytes) {
            if (ec) {
                _from(saveFile) << system_error(ec).what() << '\n';
                stop();
                return;
            }
            auto e = X::NoError;
            auto path = pt.get<xstring>("resourcePath");
            if (R::add(path, info.getBuffer(), size) && userManager.setResource(pt)) {
                _from(saveFile) << "succeed to save file to " + path << '\n';
            } else {
                _from(saveFile) << "failed to save file to " + path << '\n';
                e = X::InvalidResource;
            }
            write(e, ac, token, {});
        }
    );
}

void SocketWrapper::doLogin(const ptree &pt, const xll &token) {
    auto tr = ptree();
    auto ec = X::NoError;
    xll tk = 0;
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
            auto userid = p.get<xint>("userid", 0);
            auto priority = p.get<xint>("priority", 0);
            auto loginTime = Session::getNowTime();
            _from(doLogin) << "token: " << tk << ", userid: " << userid << ", loginTime: " << loginTime << '\n';
            if (sessionManager.add(tk, userid, loginTime + sessionManager.getDefaultAlive(), priority, true)) {
                _from(doLogin) << "succeed to login\n";
                tr = std::move(p);
                ptree t;
                t.put<xint>("userid", userid);
                t.put<xstring>("ip", socket.remote_endpoint().address().to_string());
                t.put<xll>("time", loginTime);
                userManager.recordLogin(t);
            } else {
                _from(doLogin) << "fail to login\n";
                ec = X::LoginFailed;
            }
        }
    }
    write(ec, X::LoginFeedback, tk, std::move(tr));
}

void SocketWrapper::doRegister(const ptree &pt, const xll &token) {
    if (token != 0) {
        _from(doRegister) << "token != 0\n";
        write(X::RegisterFailed, X::RegisterFeedback);
        return;
    }
    _from(doRegister);
    auto ec = userManager.registerUser(pt);
    if (!ec)
        _from(doRegister) << "succeed to register\n";
    else
        _from(doRegister) << "fail to register: " << X::what(ec) << '\n';
    write(ec, X::RegisterFeedback);
}

void SocketWrapper::doLogout(const ptree &pt, const xll &token) {
    auto ec = X::NoError;
    if (token == 0) {
        _from(doLogout) << "token = 0\n";
        ec = X::NotLogin;
    } else if (!sessionManager.removeByToken(token)) {
        _from(doLogout) << "token is invalid\n";
        ec = X::InvalidToken;
    } else
        _from(doLogout) << "succeed to logout\n";
    write(ec, X::LogoutFeedback);
}

void SocketWrapper::doModify(ptree pt, const xll &token) {
    auto ec = X::NoError;
    xll tk = 0;
    if (token == 0) {
        _from(doModify) << "token = 0\n";
        ec = X::NotLogin;
    } else {
        auto it = sessionManager.findToken(token);
        if (it == nullptr) {
            _from(doModify) << "not found session\n";
            ec = X::NotLogin;
        } else {
            tk = token;
            pt.put<xint>("userid", it->getUserid());
            _from(doModify);
            ec = userManager.modifyUser(pt);
            if (!ec)
                _from(doModify) << "succeed to modify\n";
            else
                _from(doModify) << "fail to modify: " << X::what(ec) << '\n';
        }
    }
    write(ec, X::ModifyFeedback, tk);
}

void SocketWrapper::doBorrowBook(ptree pt, const xll &token) {
    auto ec = X::NoError;
    xll tk = 0;
    if (token == 0) {
        _from(doBorrowBook) << "token == 0\n";
        ec = X::NotLogin;
    } else {
        auto it = sessionManager.findToken(token);
        if (it == nullptr) {
            _from(doBorrowBook) << "not found session\n";
            ec = X::NotLogin;
        } else {
            tk = token;
            auto userid = it->getUserid();
            auto priority = it->getPriority();
            pt.put<xint>("userid", userid);
            pt.put<xint>("priority", priority);
            pt.put<xll>("beginTime", Session::getNowTime());
            _from(doBorrowBook);
            ec = userManager.borrowBook(pt);
            if (ec == X::NoError)
                _from(doBorrowBook) << "succeed to borrow a book\n";
            else
                _from(doBorrowBook) << "fail to borrow a book: " << X::what(ec) << "\n";
        }
    }
    write(ec, X::BorrowBookFeedback, tk);
}

void SocketWrapper::doReturnBook(ptree pt, const xll &token) {
    auto ec = X::NoError;
    xll tk = 0;
    if (token == 0) {
        _from(doReturnBook) << "token == 0\n";
        ec = X::NotLogin;
    } else {
        auto it = sessionManager.findToken(token);
        if (it == nullptr) {
            _from(doReturnBook) << "not found session\n";
            ec = X::NotLogin;
        } else {
            tk = token;
            auto priority = it->getPriority();
            if (priority < X::ADMINISTER) {
                _from(doReturnBook) << "no permission\n";
                ec = X::NoPermission;
            } else {
                _from(doReturnBook);
                pt.put("returnTime", Session::getNowTime());
                ec = userManager.returnBook(pt);
                if (ec == X::NoError)
                    _from(doReturnBook) << "succeed to return a book\n";
                else
                    _from(doReturnBook) << "fail to return a book: " << X::what(ec) << "\n";
            }
        }
    }
    write(ec, X::ReturnBookFeedback, tk);
}

void SocketWrapper::doStarBook(ptree pt, const xll &token) {
    auto ec = X::NoError;
    xll tk = 0;
    if (token == 0) {
        _from(doStarBook) << "token == 0\n";
        ec = X::NotLogin;
    } else {
        auto it = sessionManager.findToken(token);
        if (it == nullptr) {
            _from(doStarBook) << "not found session\n";
            ec = X::NotLogin;
        } else {
            tk = token;
            pt.put<xint>("userid", it->getUserid());
            pt.put<xint>("priority", it->getPriority());
            pt.put<xll>("time", Session::getNowTime());
            _from(doStarBook);
            ec = userManager.starBook(pt);
            if (ec == X::NoError)
                _from(doStarBook) << "succeed to star a book\n";
            else
                _from(doStarBook) << "fail to star a book: " << X::what(ec) << "\n";
        }
    }
    write(ec, X::StarBookFeedback, tk);
}

void SocketWrapper::doUnStarBook(ptree pt, const xll &token) {
    auto ec = X::NoError;
    xll tk = 0;
    if (token == 0) {
        _from(doUnStarBook) << "token == 0\n";
        ec = X::NotLogin;
    } else {
        auto it = sessionManager.findToken(token);
        if (it == nullptr) {
            _from(doUnStarBook) << "not found session\n";
            ec = X::NotLogin;
        } else {
            tk = token;
            pt.put<xint>("userid", it->getUserid());
            _from(doUnStarBook);
            ec = userManager.unStarBook(pt);
            if (ec == X::NoError)
                _from(doUnStarBook) << "succeed to un star a book\n";
            else
                _from(doUnStarBook) << "fail to un star a book: " << X::what(ec) << "\n";
        }
    }
    write(ec, X::UnStarBookFeedback, tk);
}

void SocketWrapper::doGetBook(ptree pt, const xll &token) {
    auto tr = ptree();
    auto ec = X::NoError;
    xll tk = 0;
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
            pt.put<xint>("userid", it->getUserid());
            pt.put<xint>("priority", it->getPriority());
            pt.put<xll>("time", Session::getNowTime());
            _from(doGetBook);
            auto p = userManager.getBook(pt);
            if (p.empty()) {
                _from(doGetBook) << "fail to get a book: p is empty\n";
                ec = X::InvalidBook;
            } else {
                _from(doGetBook) << "succeed to get a book\n";
                tr = std::move(p);
            }
        }
    }
    write(ec, X::GetBookFeedback, tk, std::move(tr));
}

void SocketWrapper::doGetBookBrief(ptree pt, const xll &token) {
    auto tr = ptree();
    auto ec = X::NoError;
    xll tk = 0;
    if (token == 0) {
        _from(doGetBookBrief) << "token == 0\n";
        ec = X::NotLogin;
    } else {
        auto it = sessionManager.findToken(token);
        if (it == nullptr) {
            _from(doGetBookBrief) << "not found session\n";
            ec = X::NotLogin;
        } else {
            tk = token;
            pt.put<xint>("userid", it->getUserid());
            pt.put<xint>("priority", it->getPriority());
            _from(doGetBookBrief);
            auto p = userManager.getBookBrief(pt);
            if (p.empty()) {
                _from(doGetBookBrief) << "fail to get a book brief: p is empty\n";
                ec = X::InvalidBook;
            } else {
                _from(doGetBookBrief) << "succeed to get a book brief\n";
                tr = std::move(p);
            }
        }
    }
    write(ec, X::GetBookBriefFeedback, tk, std::move(tr));
}

void SocketWrapper::doSetBook(ptree pt, const xll &token) {
    auto ec = X::NoError;
    xll tk = 0;
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
            xint bookPriority = pt.get<xint>("priority", X::SUPER_ADMINISTER);
            auto priority = it->getPriority();
            pt.put<xint>("userPriority", priority);
            _from(doSetBook) << "userid: " << it->getUserid() << ", priority: " << priority << ", bookPriority: " << bookPriority << '\n';
            if (bookPriority > priority || priority < X::ADMINISTER) {
                _from(doSetBook) << "no permission\n";
                ec = X::NoPermission;
            } else {
                _from(doSetBook);
                ec = userManager.setBook(pt);
                if (!ec)
                    _from(doSetBook) << "succeed to set a book\n";
                else
                    _from(doSetBook) << "fail to set a book: " << X::what(ec) << '\n';
            }
        }
    }
    write(ec, X::SetBookFeedback, tk);
}

void SocketWrapper::doGetRecord(ptree pt, const xll &token, const xstring &type, const ActionCode &feedback) {
    auto tr = ptree();
    auto ec = X::NoError;
    xll tk = 0;
    if (token == 0) {
        _from(doGetRecord) << "token == 0\n";
        ec = X::NotLogin;
    } else {
        auto it = sessionManager.findToken(token);
        if (it == nullptr) {
            _from(doGetRecord) << "not found session\n";
            ec = X::NotLogin;
        } else {
            tk = token;
            pt.put<xint>("userid", it->getUserid());
            pt.put<xstring>("type", type);
            _from(doGetRecord);
            tr = userManager.getRecord(pt);
        }
    }
    write(ec, feedback, tk, std::move(tr));
}

void SocketWrapper::doGetNewBookList(ptree pt, const xll &token) {
    auto tr = ptree();
    auto ec = X::NoError;
    xll tk = 0;
    if (token == 0) {
        _from(doGetNewBookList) << "token == 0\n";
        ec = X::NotLogin;
    } else {
        auto it = sessionManager.findToken(token);
        if (it == nullptr) {
            _from(doGetNewBookList) << "not found session\n";
            ec = X::NotLogin;
        } else {
            tk = token;
            pt.put<xint>("priority", it->getPriority());
            _from(doGetNewBookList);
            tr = userManager.getNewBookList(pt);
        }
    }
    write(ec, X::GetNewBookListFeedback, tk, std::move(tr));
}

void SocketWrapper::doGetBookCover(ptree pt, const xll &token) {
    auto ec = X::NoError;
    xll tk = 0;
    char *data = 0;
    size_t dataSize = 0;
    if (token == 0) {
        _from(doGetBookCover) << "token == 0\n";
        ec = X::NotLogin;
    } else {
        auto it = sessionManager.findToken(token);
        if (it == nullptr) {
            _from(doGetBookCover) << "not found session\n";
            ec = X::NotLogin;
        } else {
            tk = token;
            pt.put<xint>("priority", it->getPriority());
            pt.put("resourceName", "bookCover");
            _from(doGetBookCover);
            auto path = userManager.getResource(pt);
            data = R::get(path, dataSize);
            if (!dataSize) {
                _from(doGetBookCover) << "failed to get the book cover\n";
                ec = X::NoSuchResource;
            } else
                _from(doGetBookCover) << "succeed to get the book cover\n";
        }
    }
    write(ec, X::GetBookCoverFeedback, tk, {}, data, dataSize);
}

void SocketWrapper::doSetBookCover(ptree pt, const xll &token) {
    auto ec = X::NoError;
    xll tk = 0;
    if (token == 0) {
        _from(doSetBookCover) << "token = 0\n";
        ec = X::NotLogin;
    } else {
        auto it = sessionManager.findToken(token);
        if (it == nullptr) {
            _from(doSetBookCover) << "not found session\n";
            ec = X::NotLogin;
        } else {
            tk = token;
            auto priority = it->getPriority();
            if (priority < X::ADMINISTER) {
                _from(doSetBookCover) << "no permission\n";
                ec = X::NoPermission;
            } else {
                auto bookid = pt.get("bookid", 0);
                auto path = "book/" + std::to_string(bookid) + "/cover.png";
                pt.put("priority", priority);
                pt.put("resourceName", "bookCover");
                pt.put("resourcePath", path);
            }
        }
    }
    saveFile(ec, X::SetBookCoverFeedback, tk, pt);
}
