// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <iostream>
#include <string>

using std::cerr;

namespace X {
    typedef unsigned int uint;
    typedef unsigned long long ull;
    typedef std::string string;

    extern const string AUTHOR[];
    extern const string EMAIL[];
    extern const string &WEBSITE;
    extern const string &GITHUB;

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
        SetBookFeedback
    };

    extern string what(const ErrorCode &ec);
    extern string what(const ActionCode &ac);
}
