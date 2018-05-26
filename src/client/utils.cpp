// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QObject>

#include <client/utils.h>

namespace X {
    QString what(const ErrorCode &ec) {
        switch (ec) {
        case NoError:
            return QObject::tr("No Error");
        case UnknownError:
            return QObject::tr("Unknown Error");
        case InvalidBody:
            return QObject::tr("Invalid Body");
        case LoginFailed:
            return QObject::tr("Login Failed");
        case NoSuchUser:
            return QObject::tr("No Such User");
        case AlreadyLogin:
            return QObject::tr("Already Login");
        case RegisterFailed:
            return QObject::tr("Register Failed");
        case InvalidUsername:
            return QObject::tr("Invalid Username");
        case InvalidNickname:
            return QObject::tr("Invalid Nickname");
        case InvalidPassword:
            return QObject::tr("Invalid Password");
        case InvalidEmail:
            return QObject::tr("Invalid Email");
        case AlreadyRegister:
            return QObject::tr("Already Register");
        case ModifyFailed:
            return QObject::tr("Modify Failed");
        case InvalidNewPassword:
            return QObject::tr("Invalid New Password");
        case NotLogin:
            return QObject::tr("No Login");
        case InvalidToken:
            return QObject::tr("Invalid Token");
        case InvalidBook:
            return QObject::tr("Invalid Book");
        case NoSuchBook:
            return QObject::tr("No Such Book");
        case NoRestBook:
            return QObject::tr("No Rest Book");
        case AlreadyHave:
            return QObject::tr("Already Have");
        case NoHave:
            return QObject::tr("No Have");
        case InvalidTime:
            return QObject::tr("Invalid Time");
        case AlreadyStar:
            return QObject::tr("Already Star");
        case NoSuchStar:
            return QObject::tr("No Such Star");
        case NoPermission:
            return QObject::tr("No Permission");
        case InvalidResource:
            return QObject::tr("Invalid Resource");
        case NoSuchResource:
            return QObject::tr("No Such Resource");
        default:
            return QObject::tr("No Such Error");
        }
    }
}