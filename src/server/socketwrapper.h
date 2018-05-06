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

    void write(const xll &token, const ptree &pt, const ActionCode &ac);
    void writeError(const ErrorCode &ec);

    void doLogin(const ptree &pt, const xll &token);
    void writeLogin(const xll &token, ptree pt, const ErrorCode &ec = X::NoError);

    void doRegister(const ptree &pt, const xll &token);
    void writeRegister(const ErrorCode &ec = X::NoError);

    void doLogout(const ptree &pt, const xll &token);
    void writeLogout(const ErrorCode &ec = X::NoError);

    void doBorrow(ptree pt, const xll &token);
    void writeBorrow(const xll &token, const ErrorCode &ec = X::NoError);

    void doGetBook(ptree pt, const xll &token);
    void writeGetBook(const xll &token, ptree pt, const ErrorCode &ec = X::NoError);

    void doSetBook(const ptree &pt, const xll &token);
    void writeSetBook(const xll &token, const ErrorCode &ec = X::NoError);

    void doGetRecord(ptree pt, const xll &token, const xstring &type, const ActionCode &feedback);
    void writeGetRecord(const xll &token, ptree pt, const ErrorCode &ec, const ActionCode &feedback);

    boost::asio::ip::tcp::socket socket;
    SocketInfo info;
    SessionManager &sessionManager;
    UserManager &userManager;
};
