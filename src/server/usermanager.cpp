// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <server/usermanager.h>

UserManager::UserManager(const char *mongo_url, const char *mongo_db_name) :
    pool(mongo::uri(mongo_url)),
    db_name(mongo_db_name) {

}

UserManager::~UserManager() {

}

bool UserManager::isUser(const uint &userid) {
    using namespace mongo;
    auto client = pool.acquire();
    auto doc = (*client)[db_name]["user"].find_one(make_document(
        kvp("userid", int(userid))
    ));
    return bool(doc);
}

bool UserManager::isUser(const string &username) {
    using namespace mongo;
    auto client = pool.acquire();
    auto doc = (*client)[db_name]["user"].find_one(make_document(
        kvp("username", bsoncxx::types::b_utf8(username))
    ));
    return bool(doc);
}

UserManager::uint UserManager::getPriority(const uint &userid) {
    using namespace mongo;
    auto client = pool.acquire();
    auto doc = (*client)[db_name]["user"].find_one(make_document(
        kvp("userid", int(userid))
    ));
    if (!doc)
        return 0;
    return uint(doc->view()["priority"].get_int32().value);
}

// return user info
UserManager::ptree UserManager::loginUser(const ptree &pt) {
    string username = pt.get("username", "");
    string password = pt.get("password", "");
    cerr << SocketInfo::encodePtree(pt, true);

    using namespace mongo;
    auto client = pool.acquire();
    mongocxx::options::find opt;
    opt.projection(make_document(
        kvp("_id", 0),
        kvp("password", 0),
        kvp("loginRecord", 0),
        kvp("borrowRecord", 0),
        kvp("browseRecord", 0)
    ));
    auto doc = (*client)[db_name]["user"].find_one(
        make_document(
            kvp("username", bsoncxx::types::b_utf8(username)),
            kvp("password", bsoncxx::types::b_utf8(password))
        ),
        opt
    );
    ptree p;
    if (doc)
        SocketInfo::decodePtree(bsoncxx::to_json(*doc, bsoncxx::ExtendedJsonMode::k_legacy), p);
    return std::move(p);
}

UserManager::ErrorCode UserManager::checkRegister(const string &username, const string &nickname, const string &password, const string &email) {
    static const std::regex patternUsername("[\\w\\.\\-]+");
    static const std::regex patternEmail("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    if (username.size() > 100 || !std::regex_match(username, patternUsername))
        return X::InvalidUsername;
    if (password.size() < 6 || password.size() > 100)
        return X::InvalidPassword;
    if (email.size() > 100 || !std::regex_match(email, patternEmail))
        return X::InvalidEmail;
    if (nickname.size() < 1 || nickname.size() > 100)
        return X::InvalidNickname;
    return X::NoError;
}

UserManager::ErrorCode UserManager::registerUser(const ptree &pt) {
    string username = pt.get("username", "");
    string nickname = pt.get("nickname", "");
    string password = pt.get("password", "");
    string email = pt.get("email", "");
    cerr << SocketInfo::encodePtree(pt, true);

    auto ec = checkRegister(username, nickname, password, email);
    if (!ec && isUser(username))
        ec = X::AlreadyRegister;
    if (!ec) {
        using namespace mongo;
        auto client = pool.acquire();
        auto info = (*client)[db_name]["info"].find_one({});
        uint userid = 1;
        if (info) {
            userid = uint(info->view()["userCount"].get_int32().value) + 1;
            (*client)[db_name]["info"].update_one(
                {},
                make_document(kvp("$set", make_document(kvp("userCount", int(userid)))))
            );
        } else {
            (*client)[db_name]["info"].insert_one(make_document(
                kvp("userCount", int(1)),
                kvp("bookCount", int(0))
            ));
        }
        (*client)[db_name]["user"].insert_one(make_document(
            kvp("userid", int(userid)),
            kvp("username", bsoncxx::types::b_utf8(username)),
            kvp("nickname", bsoncxx::types::b_utf8(nickname)),
            kvp("password", bsoncxx::types::b_utf8(password)),
            kvp("email", bsoncxx::types::b_utf8(email)),
            kvp("priority", int(AbstractUser::USER)),
            kvp("loginRecord", make_array()),
            kvp("borrowRecord", make_array()),
            kvp("browseRecord", make_array())
        ));
    }
    return ec;
}

// maybe just use pt to search book not just by bookid
UserManager::ptree UserManager::getBookCore(const ptree &pt) {
    auto bookid = pt.get<uint>("bookid", 0);
    cerr << SocketInfo::encodePtree(pt, true);

    using namespace mongo;
    auto client = pool.acquire();
    mongocxx::options::find opt;
    opt.projection(make_document(
        kvp("_id", 0),
        kvp("resource", 0)
    ));
    auto doc = (*client)[db_name]["book"].find_one(
        make_document(
            kvp("bookid", int(bookid))
        ),
        opt
    );
    ptree p;
    if (doc)
        SocketInfo::decodePtree(bsoncxx::to_json(*doc, bsoncxx::ExtendedJsonMode::k_legacy), p);
    return std::move(p);
}

UserManager::ErrorCode UserManager::setBookCore(const ptree &pt) {
    uint bookid = pt.get<uint>("bookid", 0);
    auto title = pt.get_optional<string>("title");
    auto author = pt.get_optional<string>("author");
    auto ISBN = pt.get_optional<string>("ISBN");
    auto publisher = pt.get_optional<string>("publisher");
    auto amount = pt.get_optional<uint>("amount");
    auto introduction = pt.get_optional<string>("introduction");
    auto priority = pt.get_optional<uint>("priority");
    cerr << SocketInfo::encodePtree(pt, true);

    using namespace mongo;
    auto client = pool.acquire();
    auto ec = X::NoError;
    if (bookid) {
        auto info = (*client)[db_name]["book"].find_one(make_document(
            kvp("bookid", int(bookid))
        ));
        if (info && (title || author || ISBN || publisher || amount || introduction || priority)) {
            auto doc = document();
            doc << "$set" << open_document;
            if (title)
                doc << "title" << bsoncxx::types::b_utf8(*title);
            if (author)
                doc << "author" << bsoncxx::types::b_utf8(*author);
            if (ISBN)
                doc << "ISBN" << bsoncxx::types::b_utf8(*ISBN);
            if (publisher)
                doc << "publisher" << bsoncxx::types::b_utf8(*publisher);
            if (amount)
                doc << "amount" << int(*amount);
            if (introduction)
                doc << "introduction" << bsoncxx::types::b_utf8(*introduction);
            if (priority)
                doc << "priority" << int(*priority);
            doc << close_document;
            bsoncxx::document::value val = doc << finalize;
            (*client)[db_name]["book"].update_one(
                make_document(kvp("bookid", int(bookid))),
                val.view()
            );
        } else {
            ec = X::NoSuchBook;
        }
    } else {
        auto info = (*client)[db_name]["info"].find_one({});
        bookid = 1;
        if (info) {
            bookid = uint(info->view()["bookCount"].get_int32().value) + 1;
            (*client)[db_name]["info"].update_one(
                {},
                make_document(kvp("$set", make_document(kvp("bookCount", int(bookid)))))
            );
        } else {
            (*client)[db_name]["info"].insert_one(make_document(
                kvp("bookCount", int(1)),
                kvp("userCount", int(0))
            ));
        }
        (*client)[db_name]["book"].insert_one(make_document(
            kvp("bookid", int(bookid)),
            kvp("title", bsoncxx::types::b_utf8(title ? *title : "")),
            kvp("author", bsoncxx::types::b_utf8(author ? *author : "")),
            kvp("ISBN", bsoncxx::types::b_utf8(ISBN ? *ISBN : "")),
            kvp("publisher", bsoncxx::types::b_utf8(publisher ? *publisher : "")),
            kvp("amount", int(amount ? *amount : 0)),
            kvp("introduction", bsoncxx::types::b_utf8(introduction ? *introduction : "")),
            kvp("priority", int(priority ? *priority : AbstractUser::SUPER_ADMINISTER)),
            kvp("resources", make_array())
        ));
    }
    return ec;
}
