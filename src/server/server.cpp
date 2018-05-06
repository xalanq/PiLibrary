// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <boost/thread.hpp>

#include <server/server.h>

Server::Server(boost::asio::io_service &service, const ptree &config) :
    acceptor(service, boost::asio::ip::tcp::endpoint(
        boost::asio::ip::address::from_string(config.get<xstring> ("server_url")), 
        config.get<xint> ("server_port"))),
    socket(
        service),
    thread_number(
        config.get<xint> ("thread_number")),
    userManager(
        config.get<xstring> ("mongodb_url"), 
        config.get<xstring> ("mongodb_db_name")),
    sessionManager(
        config.get<xll> ("default_alive_ms")) {
}

void Server::start(const ptree &config) {
    xint thread_number = config.get<xint>("thread_number");

    cerr << "Listening: " << config.get<xstring> ("server_url") << ":" << config.get<xint> ("server_port") << '\n';
    cerr << "mongo_url: " << config.get<xstring>("mongodb_url") << '\n';
    cerr << "mongo_db_name: " << config.get<xstring>("mongodb_db_name") << '\n';
    cerr << "Thread number: " << thread_number << '\n';
    cerr << "Default alive time: " << config.get<xll> ("default_alive_ms") << " ms\n";

    boost::asio::io_service service;
    Server server(service, config);
    server.start();

    for (int i = 0; i < thread_number - 1; ++i) {
        boost::thread(
            [&service]() {
                service.run();
            }
        );
    }
    service.run();
    cerr << "done\n";
}

void Server::start() {
    doAccept();
}

void Server::doAccept() {
    acceptor.async_accept(socket,
        [this](const error_code &ec) {
            if (!ec)
                std::make_shared<SocketWrapper> (std::move(socket), sessionManager, userManager)->start();
            doAccept();
        }
    );
}
