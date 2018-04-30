#include "server.h"

Server::Server(boost::asio::io_service &service, const X::string &url, const int &port, const X::string &mongo_url, const X::string &mongo_db_name, const X::uint &default_alive, const int &thread_number) :
    acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(url), port)),
    socket(service),
    thread_number(thread_number),
    userManager(mongo_url.c_str(), mongo_db_name.c_str()),
    sessionManager(default_alive) {

}

void Server::start(const X::string &url, const int &port, const X::string &mongo_url, const X::string &mongo_db_name, const X::uint &default_alive, const int &thread_number) {
    cerr << "Listening: " << url << ":" << port << '\n';
    cerr << "mongo_url: " << mongo_url << ", db_name: " << mongo_db_name << '\n';
    cerr << "Thread number: " << thread_number << '\n';

    boost::asio::io_service service;
    Server server(service, url, port, mongo_url, mongo_db_name, default_alive, thread_number);
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
