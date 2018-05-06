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
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;
    typedef X::ErrorCode ErrorCode;
    typedef X::ActionCode ActionCode;

    SocketWrapper(boost::asio::ip::tcp::socket socket, SessionManager &sessionManager, UserManager &userManager);
    void start();
    void stop();

private:
    void read();
    void readHeader();
    void readBody(ull token, uint length, ActionCode ac);

    void write(const ull &token, const ptree &pt, const ActionCode &ac);

    void doLogin(const ptree &pt, const ull &token);
    void writeLogin(const ull &token, ptree pt, const ErrorCode &ec = X::NoError);

    void doRegister(const ptree &pt, const ull &token);
    void writeRegister(const ErrorCode &ec = X::NoError);

    void doLogout(const ptree &pt, const ull &token);
    void writeLogout(const ErrorCode &ec = X::NoError);

    void doGetBook(ptree pt, const ull &token);
    void writeGetBook(const ull &token, ptree pt, const ErrorCode &ec = X::NoError);

    void doSetBook(const ptree &pt, const ull &token);
    void writeSetBook(const ull &token, const ErrorCode &ec = X::NoError);

    void doGetRecord(ptree pt, const ull &token, const string &type, const ActionCode &feedback);
    void writeGetRecord(const ull &token, ptree pt, const ErrorCode &ec, const ActionCode &feedback);

    boost::asio::ip::tcp::socket socket;
    SocketInfo info;
    SessionManager &sessionManager;
    UserManager &userManager;
};
