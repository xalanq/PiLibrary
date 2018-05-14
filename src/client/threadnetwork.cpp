// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <QSettings>

#include <client/threadnetwork.h>

ThreadNetwork::ThreadNetwork(QObject *parent) :
    io_service(),
    ep(boost::asio::ip::address::from_string(
          QSettings().value("Network/server_url", "127.0.0.1").toString().toStdString()),
       QSettings().value("Network/server_port", 2333).toInt()),
    QThread(parent) {
    qRegisterMetaType<ptree>("ptree");
    qRegisterMetaType<xll>("xll");
    qRegisterMetaType<xint>("xint");
    qRegisterMetaType<xstring>("xstring");
    qRegisterMetaType<ErrorCode>("ErrorCode");
    qRegisterMetaType<ActionCode>("ActionCode");
}

boost::asio::ip::tcp::socket ThreadNetwork::newSocket() {
    boost::asio::ip::tcp::socket socket(io_service);
    socket.connect(ep);
    return std::move(socket);
}
