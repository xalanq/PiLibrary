// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <iostream>
#include <string>

#include <boost/property_tree/ptree.hpp>

using std::cerr;

namespace X {
    typedef boost::property_tree::ptree ptree;
    typedef int xint;
    typedef long long xll;
    typedef std::string xstring;
    
    enum UserPriority {
        USER = 1,
        ADMINISTER = 50,
        SUPER_ADMINISTER = 100
    };

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

        NoPermission,
        
        InvalidResource,
        NoSuchResource
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
        GetNewBookListFeedback,
        GetBookCover,
        GetBookCoverFeedback,
        SetBookCover,
        SetBookCoverFeedback,
        GetSearchBookList,
        GetSearchBookListFeedback,
        SetPriority,
        SetPriorityFeedback,
        GetTopBookList,
        GetTopBookListFeedback
    };
}
