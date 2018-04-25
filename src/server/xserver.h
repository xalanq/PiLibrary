#ifndef XSERVER_H
#define XSERVER_H

#include "../core/xcore.h"

namespace X {
    const string &sql_url = "127.0.0.1";
    const string &sql_port = "2332";
    const int &default_thread_number = 32;
    const int &ping_time_limit = 5000;
}

#endif // XSERVER_H
