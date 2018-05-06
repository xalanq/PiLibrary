// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <iostream>
#include <string>
#include <regex>

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

using std::cerr;

namespace X {
    typedef unsigned int uint;
    typedef unsigned long long ull;
    typedef std::string string;

    extern const string AUTHOR[];
    extern const string EMAIL[];
    extern const string &WEBSITE;
    extern const string &GITHUB;
    extern const std::regex patternUsername;
    extern const std::regex patternEmail;

    enum ErrorCode {
        NoError = 0,

        LoginFailed,
        NoSuchUser,
        AlreadyLogin,

        RegisterFailed,
        InvalidUsername,
        InvalidNickname,
        InvalidPassword,
        InvalidEmail,
        AlreadyRegister,

        NotLogin,
        InvalidToken,

        InvalidBook,
        NoSuchBook,
        NoPermission
    };

    enum ActionCode {
        NoAction = 0,
        Login,
        LoginFeedback,
        Register,
        RegisterFeedback,
        Logout,
        LogoutFeedback,
        GetBook,
        GetBookFeedback,
        SetBook,
        SetBookFeedback,
        GetLoginRecord,
        GetLoginRecordFeedback,
        GetBorrowRecord,
        GetBorrowRecordFeedback,
        GetBrowseRecord,
        GetBrowseRecordFeedback,
        GetKeep,
        GetKeepFeedback
    };

    extern string what(const ErrorCode &ec);
    extern string what(const ActionCode &ac);

    extern void tcp_sync_read(boost::asio::ip::tcp::socket &socket, ull &token, ActionCode &ac, boost::property_tree::ptree &pt);
    extern void tcp_sync_write(boost::asio::ip::tcp::socket &socket, const ull &token, const ActionCode &ac, const boost::property_tree::ptree &pt);
}
