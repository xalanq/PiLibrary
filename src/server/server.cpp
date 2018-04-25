#include "server.h"

Server::Server(const std::string &url, const int &port, const std::string &sql_url, const int &sql_port, const int &thread_number) :
    service(),
    acceptor(service, ip::tcp::endpoint(ip::address::from_string(url), port)),
    socket(service),
    thread_number(thread_number) {

}

std::shared_ptr<Server> Server::start(const std::string &url, const int &port, const std::string &sql_url, const int &sql_port) {
    auto server = std::make_shared<Server>(new Server(url, port, sql_url, sql_port, thread_number));
    server->start();
    return server;
}

void Server::start() {
    doAccept();
    for (int i = 0; i < thread_number; ++i) {
        boost::thread(
            [service]() {
                service.run();
            }
        );
    }
}

void Server::doAccept() {
    acceptor.async_accept(socket,
        [this](const error_code &ec) {
            if (!ec)
                std::make_shared<SocketWrapper>(new SocketWrapper(std::move(socket)))->start();
            doAccept();
        }
    );
}
