// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <core/xcore.h>

namespace X {
    const string AUTHOR[] = {"xalanq", "chang-ran"};
    const string EMAIL[] = {"xalanq@gmail.com", "chang-r17@mails.tsinghua.edu.cn"};
    const string &WEBSITE = "blog.xalanq.com";
    const string &GITHUB = "github.com/xalanq/PiLibrary";

    string what(const ErrorCode &ec) {
        switch (ec) {
        case NoError:
            return "No Error";
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
        case NotLogin:
            return "No Login";
        case InvalidToken:
            return "Invalid Token";
        case InvalidBook:
            return "Invalid Book";
        case NoSuchBook:
            return "No Such Book";
        case NoPermission:
            return "No Permission";
        default:
            return "No Such Error";
        }
    }

    string what(const ActionCode &ac) {
        switch (ac) {
        case NoAction:
            return "No Action";
        case Login:
            return "Login";
        case LoginFeedback:
            return "Login Feedback";
        case Register:
            return "Register";
        case RegisterFeedback:
            return "Register Feedback";
        case Logout:
            return "Logout";
        case LogoutFeedback:
            return "Logout Feedback";
        case GetBook:
            return "Get Book";
        case GetBookFeedback:
            return "Get Book Feedback";
        case SetBook:
            return "Set Book";
        case SetBookFeedback:
            return "Set Book Feedback";
        default:
            return "No Such Action";
        }
    }

}
