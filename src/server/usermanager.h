#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "xserver.h"
#include "../core/socketinfo.h"
#include "../core/abstractuser.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/types.hpp>
#include <bson.h>

#include <mongocxx/client.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>

#include <boost/property_tree/ptree.hpp>

#include <regex>

namespace mongo {
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_document;
    using bsoncxx::builder::basic::make_array;
    using bsoncxx::stdx::string_view;
    using mongocxx::collection;
    using mongocxx::validation_criteria;
    using mongocxx::pool;
    using mongocxx::uri;
    using mongocxx::client;
}

class UserManager {
public:
    typedef X::ull ull;
    typedef X::uint uint;
    typedef X::string string;
    typedef X::ErrorCode ErrorCode;
    typedef boost::property_tree::ptree ptree;

    UserManager(const char *mongo_url, const char *mongo_db_name);
    ~UserManager();

    static string to_json(bsoncxx::document::value &document);

    bool isUser(const uint &userid);
    bool isUser(const string &username);
    uint getPriority(const uint &userid);

    ptree loginUser(const string &username, const string &password);

    ErrorCode checkRegister(const string &username, const string &nickname, const string &password, const string &email);
    ErrorCode registerUser(const string &username, const string &nickname, const string &password, const string &email);

private:
    mongo::pool pool;
    const char *db_name;
};

#endif // USERMANAGER_H
