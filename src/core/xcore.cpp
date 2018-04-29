#include "xcore.h"

namespace X {
    const string author[] = {"xalanq", "chang-ran"};
    const string email[] = {"xalanq@gmail.com", "chang-r17@mails.tsinghua.edu.cn"};
    const string &website = "blog.xalanq.com";
    const string &github = "github.com/xalanq/PiLibrary";

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
        case InvalidPassword:
            return "Invalid Password";
        case InvalidEmail:
            return "Invalid Email";
        case AlreadyRegister:
            return "Already Register";
        default:
            return "NoSuchError";
        }
    }

    string what(const ActionCode &ac) {
        switch (ac) {
        case NoAction:
            return "NoAction";
        case Login:
            return "Login";
        case LoginFeedback:
            return "LoginFeedback";
        case Register:
            return "Register";
        case RegisterFeedback:
            return "RegisterFeedback";
        default:
            return "NoSuchAction";
        }
    }

}
