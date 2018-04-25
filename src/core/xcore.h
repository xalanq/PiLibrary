#ifndef XCORE_H
#define XCORE_H

#include <string>

namespace X {
    using std::string;
    typedef unsigned int uint;
    const string author[] = {"xalanq", "changran"};
    const string email[] = {"xalanq@gmail.com", "chang-r17@mails.tsinghua.edu.cn"};
    const string &website = "blog.xalanq.com";
    const string &github = "github.com/xalanq/PiLibrary";
    const string &server_port = "2333";

    namespace ErrorCode {
        enum ErrorCode {
            LoginFailed = 1 // 登录失败
        };
    }
}

#endif // XCORE_H
