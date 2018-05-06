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
    typedef int xint;
    typedef long long xll;
    typedef std::string xstring;

    extern const xstring AUTHOR[];
    extern const xstring EMAIL[];
    extern const xstring &WEBSITE;
    extern const xstring &GITHUB;
    extern const std::regex patternUsername;
    extern const std::regex patternEmail;

    enum ErrorCode {
        NoError = 0,

        UnknownError,

        InvalidBody,

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

        NoRestBook,
        AlreadyHave,
        NoHave,
        InvalidTime,

        NoPermission
    };

    enum ActionCode {
        NoAction = 0,
        Error,
        Login,
        LoginFeedback,
        Register,
        RegisterFeedback,
        Logout,
        LogoutFeedback,
        BorrowBook,
        BorrowBookFeedback,
        ReturnBook,
        ReturnBookFeedback,
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

    extern xstring what(const ErrorCode &ec);
    extern xstring what(const ActionCode &ac);

    extern void tcp_sync_read(boost::asio::ip::tcp::socket &socket, xll &token, ActionCode &ac, boost::property_tree::ptree &pt);
    extern void tcp_sync_write(boost::asio::ip::tcp::socket &socket, const xll &token, const ActionCode &ac, const boost::property_tree::ptree &pt);
}
