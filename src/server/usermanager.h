// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#pragma once

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
#include <mongocxx/pipeline.hpp>
#include <mongocxx/uri.hpp>

#include <server/xserver.h>

namespace mongo {
    using bsoncxx::builder::stream::array;
    using bsoncxx::builder::stream::close_array;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::finalize;
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_document;
    using bsoncxx::builder::basic::make_array;
    using bsoncxx::builder::stream::open_array;
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::stdx::string_view;
    using mongocxx::client;
    using mongocxx::collection;
    using mongocxx::pool;
    using mongocxx::pipeline;
    using mongocxx::uri;
    using mongocxx::validation_criteria;
}

class UserManager {
public:
    typedef X::xll xll;
    typedef X::xint xint;
    typedef X::xstring xstring;
    typedef X::ErrorCode ErrorCode;
    typedef boost::property_tree::ptree ptree;

    UserManager(const xstring &mongo_url, const xstring &mongo_db_name);
    ~UserManager();

    bool isUser(const xint &userid);

    bool isUser(const xstring &username);

    xint getPriority(const xint &userid);

    ptree loginUser(const ptree &pt);

    ErrorCode registerUser(const ptree &pt);

    ErrorCode modifyUser(const ptree &pt);

    ErrorCode borrowBook(const ptree &pt);

    ErrorCode returnBook(const ptree &pt);

    ErrorCode starBook(const ptree &pt);

    ErrorCode unStarBook(const ptree &pt);

    ptree getBook(const ptree &pt);

    ErrorCode setBook(const ptree &pt);

    ptree getRecord(const ptree &pt);

    ptree getNewBookList(const ptree &pt);

    // not open for user, so ensure the userid exists in the below methods
    void recordLogin(const ptree &pt);

private:
    mongo::pool pool;
    xstring _db_name;
    const char *db_name;
};
