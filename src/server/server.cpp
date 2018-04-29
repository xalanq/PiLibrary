#include "server.h"

Server::Server(const X::string &url, const int &port, const X::string &mongo_url, const X::string &mongo_db_name, const X::uint &default_alive, const int &thread_number) :
    service(),
    acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(url), port)),
    socket(service),
    thread_number(thread_number),
    userManager(mongo_url.c_str(), mongo_db_name.c_str()),
    sessionManager(default_alive) {

}

std::shared_ptr<Server> Server::start(const X::string &url, const int &port, const X::string &mongo_url, const X::string &mongo_db_name, const X::uint &default_alive, const int &thread_number) {
    auto server = std::make_shared<Server> (url, port, mongo_url, mongo_db_name, default_alive, thread_number);
    cerr << "Listening: " << url << ":" << port << '\n';
    cerr << "mongo_url: " << mongo_url << ", database name:" << mongo_db_name << '\n';
    cerr << "Thread number: " << thread_number << '\n';
    server->start();
    return server;
}

void Server::start() {
    doAccept();
    for (int i = 0; i < thread_number - 1; ++i) {
        boost::thread(
            [this]() {
                service.run();
            }
        );
    }
    service.run();
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
