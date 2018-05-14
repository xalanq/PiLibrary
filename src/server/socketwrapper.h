// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

#include <core/socketinfo.h>
#include <server/sessionmanager.h>
#include <server/usermanager.h>
#include <server/xserver.h>

// one read and one write as a turn
class SocketWrapper : public std::enable_shared_from_this<SocketWrapper> {
public:
    typedef boost::system::system_error system_error;
    typedef boost::system::error_code error_code;
    typedef boost::property_tree::ptree ptree;
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;
    typedef X::ErrorCode ErrorCode;
    typedef X::ActionCode ActionCode;

    SocketWrapper(boost::asio::ip::tcp::socket socket, SessionManager &sessionManager, UserManager &userManager);
    void start();
    void stop();

private:
    void read();
    void readHeader();
    void readBody(xll token, xint length, ActionCode ac);

    void write(const ErrorCode &ec, const ActionCode &ac = X::NoAction, const xll &token = 0, ptree pt = ptree());

    void doLogin(const ptree &pt, const xll &token);
    void doRegister(const ptree &pt, const xll &token);
    void doLogout(const ptree &pt, const xll &token);
    void doModify(ptree pt, const xll &token);
    void doBorrowBook(ptree pt, const xll &token);
    void doReturnBook(ptree pt, const xll &token);
    void doStarBook(ptree pt, const xll &token);
    void doUnStarBook(ptree pt, const xll &token);
    void doGetBook(ptree pt, const xll &token);
    void doSetBook(const ptree &pt, const xll &token);
    void doGetRecord(ptree pt, const xll &token, const xstring &type, const ActionCode &feedback);
    void doGetNewBookList(ptree pt, const xll &token);

    boost::asio::ip::tcp::socket socket;
    SocketInfo info;
    SessionManager &sessionManager;
    UserManager &userManager;
};
