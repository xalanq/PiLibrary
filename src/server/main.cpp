#include <iostream>
#include <fstream>
#include "../core/xcore.h"
#include "server.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace std;
using namespace boost::property_tree;

int main() {
    ptree pt;
    ifstream f("config.json");
    bool ok = bool(f);
    if (ok)
        read_json("config.json", pt);
    else {
        cerr << "Config file is not exist, build new one[./config.json]\n";
        pt.put("core.server_url", "127.0.0.1");
        pt.put("core.server_port", 2333);
        pt.put("server.sql_url", "127.0.0.1");
        pt.put("server.sql_port", 23332);
        pt.put("server.thread_number", 32);
        write_json("config.json", pt);
    }
    Server::start(
        pt.get<string> ("core.server_url", "127.0.0.1"),
        pt.get<int> ("core.server_port", 2333),
        pt.get<string> ("server.sql_url", "127.0.0.1"),
        pt.get<int> ("server.sql_port", 23332),
        pt.get<int> ("server.thread_number", 32)
    );
    return 0;
}
