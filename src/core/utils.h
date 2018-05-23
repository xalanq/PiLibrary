// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <boost/asio.hpp>

#include <core/Resource.h>
#include <core/types.h>

namespace X {
    extern xstring what(const ErrorCode &ec);
    extern xstring what(const ActionCode &ac);

    extern bool checkUsername(const xstring &username);
    extern bool checkNickname(const xstring &nickname);
    extern bool checkPassword(const xstring &password);
    extern bool checkEmail(const xstring &email);

    extern xstring time_to_str(const xll &time);

    extern void tcp_sync_read(boost::asio::ip::tcp::socket &socket, xll &token, ActionCode &ac, ptree &pt);
    extern Resource tcp_sync_read_with_file(boost::asio::ip::tcp::socket &socket, xll &token, ActionCode &ac, ptree &pt);
    extern void tcp_sync_write(boost::asio::ip::tcp::socket &socket, const xll &token, const ActionCode &ac, const ptree &pt);
    extern void tcp_sync_write_with_file(boost::asio::ip::tcp::socket &socket, const xll &token, const ActionCode &ac, const ptree &pt, const Resource &file);
}