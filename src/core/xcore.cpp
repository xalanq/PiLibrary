// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <boost/property_tree/json_parser.hpp>

#include <core/socketinfo.h>
#include <core/xcore.h>

namespace X {
    const xstring AUTHOR[] = {"xalanq", "chang-ran"};
    const xstring EMAIL[] = {"xalanq@gmail.com", "chang-r17@mails.tsinghua.edu.cn"};
    const xstring &WEBSITE = "blog.xalanq.com";
    const xstring &GITHUB = "github.com/xalanq/PiLibrary";
    const std::regex patternUsername("[\\w\\.\\-]+");
    const std::regex patternEmail("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

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
        case NoPermission:
            return "No Permission";
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
        case Logout:
            return "Logout";
        case LogoutFeedback:
            return "Logout Feedback";
        case BorrowBook:
            return "BorrowBook";
        case BorrowBookFeedback:
            return "BorrowBook Feedback";
        case ReturnBook:
            return "ReturnBook";
        case ReturnBookFeedback:
            return "ReturnBook Feedback";
        case GetBook:
            return "Get Book";
        case GetBookFeedback:
            return "Get Book Feedback";
        case SetBook:
            return "Set Book";
        case SetBookFeedback:
            return "Set Book Feedback";
        case GetLoginRecord:
            return "Get Login Record";
        case GetLoginRecordFeedback:
            return "Get Login Record Feedback";
        case GetBorrowRecord:
            return "Get BorrowBook Record";
        case GetBorrowRecordFeedback:
            return "Get BorrowBook Record Feedback";
        case GetBrowseRecord:
            return "Get Browse Record";
        case GetBrowseRecordFeedback:
            return "Get Browse Record Feedback";
        case GetKeep:
            return "Get Keep";
        case GetKeepFeedback:
            return "Get Keep Keepback";
        default:
            return "No Such Action";
        }
    }

    void tcp_sync_read(boost::asio::ip::tcp::socket &socket, xll &token, ActionCode &ac, boost::property_tree::ptree &pt) {
        SocketInfo info;

        info.setSize(SocketInfo::HEADER_SIZE);

        boost::asio::read(
            socket,
            boost::asio::buffer(info.getBuffer(), 1),
            boost::asio::transfer_exactly(1)
        );
        boost::asio::read(
            socket,
            boost::asio::buffer(info.getBuffer(), SocketInfo::HEADER_SIZE),
            boost::asio::transfer_exactly(SocketInfo::HEADER_SIZE)
        );

        token = info.decodeHeaderToken();
        auto length = info.decodeHeaderLength();
        ac = info.decodeHeaderActionCode();

        info.setSize(length);

        boost::asio::read(
            socket,
            boost::asio::buffer(info.getBuffer(), length),
            boost::asio::transfer_exactly(length)
        );

        info.decodeBody(length, pt);
    }

    void tcp_sync_write(boost::asio::ip::tcp::socket &socket, const xll &token, const ActionCode &ac, const boost::property_tree::ptree &pt) {
        SocketInfo info;
        auto str = SocketInfo::encodePtree(pt);
        auto size = SocketInfo::HEADER_SIZE + 1 + str.size();

        info.setSize(size);
        info.encode(token, static_cast<xint> (str.size()), ac, str);

        boost::asio::write(
            socket,
            boost::asio::buffer(info.getBuffer(), size)
        );
    }
}
