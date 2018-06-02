// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <boost/thread.hpp>

#include <server/core/Server.h>

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

    cout << "Listening: " << config.get<xstring> ("server_url") << ":" << config.get<xint> ("server_port") << '\n';
    cout << "mongo_url: " << config.get<xstring>("mongodb_url") << '\n';
    cout << "mongo_db_name: " << config.get<xstring>("mongodb_db_name") << '\n';
    cout << "Thread number: " << thread_number << '\n';
    cout << "Default alive time: " << config.get<xll> ("default_alive_ms") << " ms\n";

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
    cout << "done\n";
}

void Server::start() {
    initRoot();
    doAccept();
}

void Server::doAccept() {
    acceptor.async_accept(socket,
        [this](const error_code &ec) {
            if (!ec) {
                try {
                    std::make_shared<SocketManager> (std::move(socket), sessionManager, userManager)->start();
                } catch (std::exception &e) {
                    cerr << "Error Message: " << e.what() << '\n';
                }
            }
            doAccept();
        }
    );
}

void Server::initRoot() {
    cout << "Linking mongodb...\n";
    try {
        bool empty = userManager.isDBEmpty();
        cout << "Mongodb is available!\n";
        if (empty) {
            cout << "\n=============Attention=============\n";
            cout << "Database is empty, create a Super Administer user.\n";
            cout << "Username: \t\t PiRoot\n";
            cout << "Default Password: \t pi123456\n";
            cout << "Please change the password as soon as possible.\n";
            cout << "===================================\n\n";
            ptree pt;
            pt.put("username", "PiRoot");
            pt.put("nickname", "root");
            pt.put("password", "2d515aaf7a77cb6744bf91a49af70928bfb9f04b");
            pt.put("email", "root@pi.com");
            userManager.registerUser(pt, X::SUPER_ADMINISTER);
        }
    } catch (std::exception &e) {
        cout << "Error: Mongodb is unavailable! Please checkout the mongodb's uri.\n";
        cout << "Error Message: " << e.what() << '\n';
        exit(0);
    }
}
