// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

#include <regex>

#include <boost/property_tree/ptree.hpp>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>

#include <core/abstractuser.h>
#include <core/socketinfo.h>
#include <server/xserver.h>

namespace mongo {
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_document;
    using bsoncxx::builder::basic::make_array;
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::array;
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::open_array;
    using bsoncxx::builder::stream::close_array;
    using bsoncxx::builder::stream::finalize;
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

    UserManager(const string &mongo_url, const string &mongo_db_name);
    ~UserManager();

    bool isUser(const uint &userid);
    bool isUser(const string &username);

    uint getPriority(const uint &userid);

    ptree loginUser(const ptree &pt);

    ErrorCode checkRegister(const string &username, const string &nickname, const string &password, const string &email);
    ErrorCode registerUser(const ptree &pt);

    ptree getBookCore(const ptree &pt);
    ErrorCode setBookCore(const ptree &pt);

private:
    mongo::pool pool;
    string _db_name;
    const char *db_name;
};
