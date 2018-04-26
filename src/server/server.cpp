#include "server.h"

Server::Server(const X::string &url, const int &port, const X::string &sql_url, const int &sql_port, const int &thread_number) :
    service(),
    acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(url), port)),
    socket(service),
    thread_number(thread_number) {

}

std::shared_ptr<Server> Server::start(const X::string &url, const int &port, const X::string &sql_url, const int &sql_port, const int &thread_number) {
    auto server = std::make_shared<Server> (url, port, sql_url, sql_port, thread_number);
    cerr << "Listening: " << url << ":" << port << '\n';
    cerr << "SQL: " << sql_url << ":" << sql_port << '\n';
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
                std::make_shared<SocketWrapper> (std::move(socket))->start();
            doAccept();
        }
    );
}
