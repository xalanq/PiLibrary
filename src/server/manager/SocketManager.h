// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <boost/asio.hpp>

#include <core/SocketInfo.h>
#include <core/Resource.h>
#include <core/values.h>
#include <server/manager/SessionManager.h>
#include <server/manager/UserManager.h>

// one read and one write as a turn
class SocketManager : public std::enable_shared_from_this<SocketManager> {
public:
    typedef boost::system::system_error system_error;
    typedef boost::system::error_code error_code;
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::ptree ptree;
    typedef X::xstring xstring;
    typedef X::ErrorCode ErrorCode;
    typedef X::ActionCode ActionCode;

    SocketManager(boost::asio::ip::tcp::socket socket, SessionManager &sessionManager, UserManager &userManager);
    void start();
    void stop();

private:
    void read();
    void readHeader();
    void readBody(const xll &token, const xint &length, const ActionCode &ac);

    void write(const ErrorCode &ec, const ActionCode &ac = X::NoAction, const xll &token = 0, ptree pt = ptree(), const Resource &file = Resource());

    void saveFile(const ErrorCode &ec, const ActionCode &ac, const xll &token, const ptree &pt);

    void doLogin(const ptree &pt, const xll &token);
    void doRegister(const ptree &pt, const xll &token);
    void doLogout(const ptree &pt, const xll &token);
    void doModify(ptree pt, const xll &token);
    
    void doBorrowBook(ptree pt, const xll &token);
    void doReturnBook(ptree pt, const xll &token);
    
    void doStarBook(ptree pt, const xll &token);
    void doUnStarBook(ptree pt, const xll &token);
    
    void doGetBook(ptree pt, const xll &token);
    void doGetBookBrief(ptree pt, const xll &token);
    void doSetBook(ptree pt, const xll &token);
    
    void doGetRecord(ptree pt, const xll &token, const xstring &type, const ActionCode &feedback);
    
    void doGetNewBookList(ptree pt, const xll &token);

    void doGetBookCover(ptree pt, const xll &token);
    void doSetBookCover(ptree pt, const xll &token);

    void doGetSearchBookList(ptree pt, const xll &token);

    void doSetPriority(ptree pt, const xll &token);

    boost::asio::ip::tcp::socket socket;
    SocketInfo info;
    SessionManager &sessionManager;
    UserManager &userManager;
};
