// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <iostream>
#include <string>
#include <regex>

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

using std::cerr;
typedef boost::property_tree::ptree ptree;

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

        ModifyFailed,
        InvalidNewPassword,

        NotLogin,
        InvalidToken,

        InvalidBook,
        NoSuchBook,

        NoRestBook,
        AlreadyHave,
        NoHave,
        InvalidTime,

        AlreadyStar,
        NoSuchStar,

        NoPermission
    };

    enum ActionCode {
        NoAction = 0,
        Error,
        Login,
        LoginFeedback,
        Register,
        RegisterFeedback,
        Modify,
        ModifyFeedback,
        Logout,
        LogoutFeedback,
        BorrowBook,
        BorrowBookFeedback,
        ReturnBook,
        ReturnBookFeedback,
        StarBook,
        StarBookFeedback,
        UnStarBook,
        UnStarBookFeedback,
        GetBook,
        GetBookFeedback,
        GetBookBrief,
        GetBookBriefFeedback,
        SetBook,
        SetBookFeedback,
        GetLoginRecord,
        GetLoginRecordFeedback,
        GetStarRecord,
        GetStarRecordFeedback,
        GetBorrowRecord,
        GetBorrowRecordFeedback,
        GetBrowseRecord,
        GetBrowseRecordFeedback,
        GetKeepRecord,
        GetKeepRecordFeedback,
        GetNewBookList,
        GetNewBookListFeedback
    };

    extern xstring what(const ErrorCode &ec);
    extern xstring what(const ActionCode &ac);

    extern bool checkUsername(const xstring &username);
    extern bool checkNickname(const xstring &nickname);
    extern bool checkPassword(const xstring &password);
    extern bool checkEmail(const xstring &email);

    extern xstring time_to_str(const xll &time);

    extern void tcp_sync_read(boost::asio::ip::tcp::socket &socket, xll &token, ActionCode &ac, ptree &pt);
    extern void tcp_sync_write(boost::asio::ip::tcp::socket &socket, const xll &token, const ActionCode &ac, const ptree &pt);
}
