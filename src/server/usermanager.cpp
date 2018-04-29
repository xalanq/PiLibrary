#include "usermanager.h"

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

// return userid
UserManager::uint UserManager::loginUser(const string &username, const string &password) {
    using namespace mongo;
    auto client = pool.acquire();
    auto doc = (*client)[db_name]["user"].find_one(make_document(
        kvp("username", bsoncxx::types::b_utf8(username)),
        kvp("password", bsoncxx::types::b_utf8(password))
    ));
    if (!doc)
        return 0;
    return uint(doc->view()["userid"].get_int32().value);
}

UserManager::ErrorCode UserManager::checkRegister(const string &username, const string &password, const string &email) {
    static const std::regex patternUsername("[\\w\\.\\-]+");
    static const std::regex patternEmail("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    if (username.size() > 100 || !std::regex_match(username, patternUsername))
        return X::InvalidUsername;
    if (password.size() < 6 || password.size() > 100)
        return X::InvalidPassword;
    if (email.size() > 100 || !std::regex_match(email, patternEmail))
        return X::InvalidEmail;
    return X::NoError;
}

UserManager::ErrorCode UserManager::registerUser(const string &username, const string &password, const string &email) {
    auto ec = checkRegister(username, password, email);
    if (!ec && isUser(username))
        ec = X::AlreadyRegister;
    if (!ec) {
        using namespace mongo;
        auto client = pool.acquire();
        auto info = (*client)[db_name]["info"].find_one({});
        uint userid = 1;
        if (info) {
            userid = uint(info->view()["user_count"].get_int32().value) + 1;
            (*client)[db_name]["info"].update_one(
                {},
                make_document(kvp("$set", make_document(kvp("user_count", int(userid)))))
            );
        } else {
            (*client)[db_name]["info"].insert_one(make_document(
                kvp("user_count", int(1))
            ));
        }
        auto res = (*client)[db_name]["user"].insert_one(make_document(
            kvp("userid", int(userid)),
            kvp("username", bsoncxx::types::b_utf8(username)),
            kvp("password", bsoncxx::types::b_utf8(password)),
            kvp("email", bsoncxx::types::b_utf8(email)),
            kvp("priority", int(AbstractUser::User))
        ));
    }
    return ec;
}
