// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <server/utils.h>

namespace X {
    xstring what(const ErrorCode &ec) {
        switch (ec) {
        case NoError:
            return "No Error";
        case UnknownError:
            return "Unknown Error";
        case InvalidBody:
            return "Invalid Body";
        case LoginFailed:
            return "Login Failed";
        case NoSuchUser:
            return "No Such User";
        case AlreadyLogin:
            return "Already Login";
        case RegisterFailed:
            return "Register Failed";
        case InvalidUsername:
            return "Invalid Username";
        case InvalidNickname:
            return "Invalid Nickname";
        case InvalidPassword:
            return "Invalid Password";
        case InvalidEmail:
            return "Invalid Email";
        case AlreadyRegister:
            return "Already Register";
        case ModifyFailed:
            return "Modify Failed";
        case InvalidNewPassword:
            return "Invalid New Password";
        case NotLogin:
            return "No Login";
        case InvalidToken:
            return "Invalid Token";
        case InvalidBook:
            return "Invalid Book";
        case NoSuchBook:
            return "No Such Book";
        case NoRestBook:
            return "No Rest Book";
        case AlreadyHave:
            return "Already Have";
        case NoHave:
            return "No Have";
        case InvalidTime:
            return "Invalid Time";
        case AlreadyStar:
            return "Already Star";
        case NoSuchStar:
            return "No Such Star";
        case NoPermission:
            return "No Permission";
        case InvalidResource:
            return "Invalid Resource";
        case NoSuchResource:
            return "No Such Resource";
        default:
            return "No Such Error";
        }
    }

    xstring what(const ActionCode &ac) {
        switch (ac) {
        case NoAction:
            return "No Action";
        case Error:
            return "Error";
        case Login:
            return "Login";
        case LoginFeedback:
            return "Login Feedback";
        case Register:
            return "Register";
        case RegisterFeedback:
            return "Register Feedback";
        case Modify:
            return "Modify";
        case ModifyFeedback:
            return "Modify Feedback";
        case Logout:
            return "Logout";
        case LogoutFeedback:
            return "Logout Feedback";
        case BorrowBook:
            return "Borrow Book";
        case BorrowBookFeedback:
            return "Borrow Book Feedback";
        case ReturnBook:
            return "Return Book";
        case ReturnBookFeedback:
            return "Return Book Feedback";
        case StarBook:
            return "Star Book";
        case StarBookFeedback:
            return "Star Book Feedback";
        case UnStarBook:
            return "Un Star Book";
        case UnStarBookFeedback:
            return "Un Star Book Feedback";
        case GetBook:
            return "Get Book";
        case GetBookFeedback:
            return "Get Book Feedback";
        case GetBookBrief:
            return "Get Book Brief";
        case GetBookBriefFeedback:
            return "Get Book Brief Feedback";
        case SetBook:
            return "Set Book";
        case SetBookFeedback:
            return "Set Book Feedback";
        case GetLoginRecord:
            return "Get Login Record";
        case GetLoginRecordFeedback:
            return "Get Login Record Feedback";
        case GetStarRecord:
            return "Get Star Record";
        case GetStarRecordFeedback:
            return "Get Star Record Feedback";
        case GetBorrowRecord:
            return "Get Borrow Record";
        case GetBorrowRecordFeedback:
            return "Get Borrow Record Feedback";
        case GetBrowseRecord:
            return "Get Browse Record";
        case GetBrowseRecordFeedback:
            return "Get Browse Record Feedback";
        case GetKeepRecord:
            return "Get Keep Record";
        case GetKeepRecordFeedback:
            return "Get Keep Record Feedback";
        case GetNewBookList:
            return "Get New Book List";
        case GetNewBookListFeedback:
            return "Get New Book List Feedback";
        case GetBookCover:
            return "Get Book Cover";
        case GetBookCoverFeedback:
            return "Get Book Cover Feedback";
        case SetBookCover:
            return "Set Book Cover";
        case SetBookCoverFeedback:
            return "Set Book Cover Feedback";
        case GetSearchBookList:
            return "Get Search Book List";
        case GetSearchBookListFeedback:
            return "Get Search Book List Feedback";
        case SetPriority:
            return "Set Priority";
        case SetPriorityFeedback:
            return "Set Priority Feedback";
        default:
            return "No Such Action";
        }
    }
}