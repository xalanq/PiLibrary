#ifndef XCORE_H
#define XCORE_H

#include <string>
#include <iostream>

using std::cerr;

namespace X {
    using std::string;
    typedef unsigned int uint;
    extern const string author[];
    extern const string email[];
    extern const string &website;
    extern const string &github;

    namespace ErrorCode {
        enum ErrorCode {
            LoginFailed = 1 // 登录失败
        };
    }
}

#endif // XCORE_H
