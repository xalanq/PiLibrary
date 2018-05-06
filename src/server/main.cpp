// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <iostream>
#include <fstream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <mongocxx/instance.hpp>

#include <server/server.h>
#include <server/xserver.h>

using namespace std;

int main() {
    cerr << "===================================\n";
    cerr << X::APP_NAME << " v" << X::VERSION << '\n';
    cerr << "Author: " << X::AUTHOR[0] << ", " << X::AUTHOR[1] << '\n';
    cerr << "Website: " << X::WEBSITE << '\n';
    cerr << "GitHub: " << X::GITHUB << '\n';
    cerr << "===================================\n";
    cerr << '\n';

    mongocxx::instance _ {};
    boost::property_tree::ptree config;

    ifstream f("config.json");
    if (f)
        boost::property_tree::read_json("config.json", config);
    else
        cerr << "Config file is not exist, create new one[./config.json]\n";
    if (!config.get_optional<X::xstring>("server_url"))
        config.put("server_url", "127.0.0.1");
    if (!config.get_optional<X::xint>("server_port"))
        config.put("server_port", 2333);
    if (!config.get_optional<X::xstring>("mongodb_url"))
        config.put("mongodb_url", "mongodb://localhost:23332/?minPoolSize=32&maxPoolSize=32");
    if (!config.get_optional<X::xstring>("mongodb_db_name"))
        config.put("mongodb_db_name", "pi");
    if (!config.get_optional<X::xll>("default_alive_ms"))
        config.put<X::xll>("default_alive_ms", 30000ll);
    if (!config.get_optional<X::xint>("thread_number"))
        config.put<X::xint>("thread_number", 32);
    boost::property_tree::write_json("config.json", config);
    Server::start(config);
    return 0;
}
