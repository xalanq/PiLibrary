#include <iostream>
#include <fstream>
#include "../core/xcore.h"
#include "server.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <mongocxx/instance.hpp>

using namespace std;
using namespace boost::property_tree;

int main() {
    mongocxx::instance _ {};
    /*
    mongocxx::uri uri {"mongodb://localhost:23332/?minPoolSize=32&maxPoolSize=32"};
    mongocxx::pool pool {uri};
    auto client = pool.acquire();
    auto doc = (*client)["pi"]["user"].find_one({});
    cerr << doc->view()["username"].get_utf8().value;
    while (1);
    return 0;
    */
    ptree pt;
    ifstream f("config.json");
    bool ok = bool(f);
    if (ok)
        read_json("config.json", pt);
    else {
        cerr << "Config file is not exist, build new one[./config.json]\n";
        pt.put("core.server_url", "127.0.0.1");
        pt.put("core.server_port", 2333);
        pt.put("server.mongodb_url", "mongodb://localhost:23332/?minPoolSize=32&maxPoolSize=32");
        pt.put("server.mongodb_db_name", "pi");
        pt.put("server.default_alive_ms", 30000);
        pt.put("server.thread_number", 32);
        write_json("config.json", pt);
    }
    Server::start(
        pt.get<string> ("core.server_url", "127.0.0.1"),
        pt.get<int> ("core.server_port", 2333),
        pt.get<string> ("server.mongodb_url", "mongodb://localhost:23332/?minPoolSize=32&maxPoolSize=32"),
        pt.get<string> ("server.mongodb_db_name", "pi"),
        pt.get<int> ("server.default_alive_ms", 30000),
        pt.get<int> ("server.thread_number", 32)
    );
    return 0;
}
