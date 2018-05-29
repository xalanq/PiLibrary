// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <ctime>
#include <regex>

#include <core/SocketInfo.h>
#include <core/utils.h>

namespace X {
    bool checkUsername(const xstring &username) {
        static const std::regex patternUsername("[\\w\\.\\-]+");
        if (username.size() < 1 || username.size() > 100 || !std::regex_match(username, patternUsername))
            return false;
        return true;
    }

    bool checkNickname(const xstring &nickname) {
        if (nickname.size() < 1 || nickname.size() > 100)
            return false;
        return true;
    }

    bool checkPassword(const xstring &password) {
        if (password.size() < 6 || password.size() > 100)
            return false;
        return true;
    }

    bool checkEmail(const xstring &email) {
        static const std::regex patternEmail("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
        if (email.size() < 5 || email.size() > 100 || !std::regex_match(email, patternEmail))
            return false;
        return true;
    }

    xstring escape(const xstring &str) {
        static const std::regex specialChars(R"([.^$|()\[\]{}*+?\\\&])");
        return std::regex_replace(str, specialChars, R"(\$&)");
    }

    xstring time_to_str(const xll &time) {
        time_t t = time;
        xstring str = asctime(localtime(&t));
        str.pop_back();
        return str;
    }

    void tcp_sync_read(boost::asio::ip::tcp::socket &socket, xll &token, ActionCode &ac, ptree &pt) {
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

    Resource tcp_sync_read_with_file(boost::asio::ip::tcp::socket &socket, xll &token, ActionCode &ac, ptree &pt) {
        tcp_sync_read(socket, token, ac, pt);
        Resource file;
        auto size = pt.get<size_t>("fileSize", 0);
        
        if (size) {
            SocketInfo info;
            info.setSize(size);

            boost::asio::read(
                socket,
                boost::asio::buffer(info.getBuffer(), size),
                boost::asio::transfer_exactly(size)
            );
            
            file.setData(info.getBuffer()).setSize(size);
            info.setBuffer(nullptr);
        }
        return file;
    }

    void tcp_sync_write(boost::asio::ip::tcp::socket &socket, const xll &token, const ActionCode &ac, const ptree &pt) {
        SocketInfo info;
        auto str = SocketInfo::encodePtree(pt);
        auto bodySize = xint(str.size());
        auto mainSize = SocketInfo::HEADER_SIZE + 1 + bodySize;

        info.setSize(mainSize);
        info.encodeMain(token, bodySize, ac, str);

        boost::asio::write(
            socket,
            boost::asio::buffer(info.getBuffer(), mainSize)
        );
    }

    void tcp_sync_write_with_file(boost::asio::ip::tcp::socket &socket, const xll &token, const ActionCode &ac, const ptree &pt, const Resource &file) {
        auto fileSize = file.getSize();
        if (fileSize) {
            SocketInfo info;
            auto str = SocketInfo::encodePtree(pt);
            auto bodySize = xint(str.size());
            auto mainSize = SocketInfo::HEADER_SIZE + 1 + bodySize;
            auto size = mainSize + fileSize;

            info.setSize(size);
            info.encodeMain(token, bodySize, ac, str);
            info.encodeFile(mainSize, file.getData(), fileSize);

            boost::asio::write(
                socket,
                boost::asio::buffer(info.getBuffer(), size)
            );
        }
    }

}