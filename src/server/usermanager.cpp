// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <regex>

#include <server/usermanager.h>

UserManager::UserManager(const xstring &mongo_url, const xstring &mongo_db_name) :
    pool(mongo::uri(mongo_url)),
    _db_name(mongo_db_name),
    db_name(_db_name.c_str()) {

}

UserManager::~UserManager() {

}

bool UserManager::isUser(const xint &userid) {
    cerr << "userid: " << userid << '\n';
    using namespace mongo;
    auto client = pool.acquire();
    auto doc = (*client)[db_name]["user"].find_one(
        make_document(
            kvp("userid", userid)
        )
    );
    return bool(doc);
}

bool UserManager::isUser(const xstring &username) {
    cerr << "username: " << username << '\n';
    using namespace mongo;
    auto client = pool.acquire();
    auto doc = (*client)[db_name]["user"].find_one(
        make_document(
            kvp("username", bsoncxx::types::b_utf8(username))
        )
    );
    return bool(doc);
}

UserManager::xint UserManager::getPriority(const xint &userid) {
    cerr << "userid: " << userid << '\n';
    using namespace mongo;
    auto client = pool.acquire();
    auto doc = (*client)[db_name]["user"].find_one(
        make_document(
            kvp("userid", userid)
        )
    );
    if (!doc)
        return 0;
    return xint(doc->view()["priority"].get_int32().value);
}

// return user info
UserManager::ptree UserManager::loginUser(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    xstring username = pt.get("username", "");
    xstring password = pt.get("password", "");

    using namespace mongo;
    auto client = pool.acquire();
    mongocxx::options::find opt;
    opt.projection(
        make_document(
            kvp("_id", 0),
            kvp("password", 0),
            kvp("loginRecord", 0),
            kvp("borrowRecord", 0),
            kvp("keep", 0),
            kvp("browseRecord", 0)
        )
    );
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

UserManager::ErrorCode UserManager::checkRegister(const xstring &username, const xstring &nickname, const xstring &password, const xstring &email) {
    if (username.size() < 1 || username.size() > 100 || !std::regex_match(username, X::patternUsername))
        return X::InvalidUsername;
    if (nickname.size() < 1 || nickname.size() > 100)
        return X::InvalidNickname;
    if (password.size() < 6 || password.size() > 100)
        return X::InvalidPassword;
    if (email.size() < 5 || email.size() > 100 || !std::regex_match(email, X::patternEmail))
        return X::InvalidEmail;
    return X::NoError;
}

UserManager::ErrorCode UserManager::registerUser(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    xstring username = pt.get("username", "");
    xstring nickname = pt.get("nickname", "");
    xstring password = pt.get("password", "");
    xstring email = pt.get("email", "");

    auto ec = checkRegister(username, nickname, password, email);
    if (!ec && isUser(username))
        ec = X::AlreadyRegister;
    if (!ec) {
        using namespace mongo;
        auto client = pool.acquire();
        auto info = (*client)[db_name]["info"].find_one({});
        xint userid = 1;
        if (info) {
            userid = xint(info->view()["userCount"].get_int32().value) + 1;
            (*client)[db_name]["info"].update_one(
                {},
                make_document(
                    kvp("$set", make_document(
                        kvp("userCount", userid)
                    ))
                )
            );
        } else {
            (*client)[db_name]["info"].insert_one(
                make_document(
                    kvp("userCount", 1),
                    kvp("bookCount", 0)
                )
            );
        }
        (*client)[db_name]["user"].insert_one(
            make_document(
                kvp("userid", userid),
                kvp("username", bsoncxx::types::b_utf8(username)),
                kvp("nickname", bsoncxx::types::b_utf8(nickname)),
                kvp("password", bsoncxx::types::b_utf8(password)),
                kvp("email", bsoncxx::types::b_utf8(email)),
                kvp("priority", int(AbstractUser::USER)),
                kvp("loginRecord", make_array()),
                kvp("borrowRecord", make_array()),
                kvp("keep", make_array()),
                kvp("browseRecord", make_array())
            )
        );
    }
    return ec;
}

// maybe need to verify time
UserManager::ErrorCode UserManager::borrowBook(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto userid = pt.get<xint>("userid");
    auto priority = pt.get<xint>("priority");
    auto bookid = pt.get<xint>("bookid", 0);
    auto beginTime = pt.get<xll>("beginTime", 1);
    auto endTime = pt.get<xll>("endTime", 0);

    if (beginTime >= endTime)
        return X::InvalidTime;

    using namespace mongo;
    auto client = pool.acquire();
    mongo::pipeline p;
    p.match(
        make_document(
            kvp("bookid", bookid),
            kvp("priority", make_document(
                kvp("$lte", priority)
            ))
    ));
    p.project(
        make_document(
            kvp("_id", 0),
            kvp("amount", 1),
            kvp("count", make_document(
                kvp("$size", "$keep")
            ))
        )
    );
    auto cur = (*client)[db_name]["book"].aggregate(p);
    ErrorCode ec = X::NoSuchBook;
    if (cur.begin() != cur.end()) {
        auto view = *cur.begin();
        auto amount = xint(view["amount"].get_int32().value);
        auto count = xint(view["count"].get_int32().value);
        if (count + 1 > amount)
            ec = X::NoRestBook;
        else {
            mongocxx::options::find opt;
            opt.projection(
                make_document(
                    kvp("_id", 0),
                    kvp("bookid", 1)
                )
            );
            auto doc = (*client)[db_name]["book"].find_one(
                make_document(
                    kvp("bookid", bookid),
                    kvp("keep.userid", userid)
                )
            );
            if (doc)
                ec = X::AlreadyHave;
            else {
                (*client)[db_name]["user"].update_one(
                    make_document(
                        kvp("userid", userid)
                    ),
                    make_document(
                        kvp("$push", make_document(
                            kvp("keep", make_document(
                                kvp("bookid", bookid),
                                kvp("beginTime", beginTime),
                                kvp("endTime", endTime)
                            )),
                            kvp("borrowRecord", make_document(
                                kvp("bookid", bookid),
                                kvp("beginTime", beginTime),
                                kvp("endTime", endTime),
                                kvp("returnTime", X::xll(0))
                            ))
                        ))
                    )
                );
                (*client)[db_name]["book"].update_one(
                    make_document(
                        kvp("bookid", bookid)
                    ),
                    make_document(
                        kvp("$push", make_document(
                            kvp("keep", make_document(
                                kvp("userid", userid),
                                kvp("beginTime", beginTime),
                                kvp("endTime", endTime)
                            )),
                            kvp("borrowRecord", make_document(
                                kvp("userid", userid),
                                kvp("beginTime", beginTime),
                                kvp("endTime", endTime),
                                kvp("returnTime", X::xll(0))
                            ))
                        ))
                    )
                );
                ec = X::NoError;
            }
        }
    }
    return ec;
}

UserManager::ErrorCode UserManager::returnBook(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto returnTime = pt.get<xll>("returnTime");
    auto userid = pt.get<xint>("userid", 0);
    auto bookid = pt.get<xint>("bookid", 0);

    using namespace mongo;
    auto client = pool.acquire();
    auto res = (*client)[db_name]["book"].update_one(
        make_document(
            kvp("bookid", bookid)
        ),
        make_document(
            kvp("$pull", make_document(
                kvp("keep", make_document(
                    kvp("userid", userid)
                ))
            ))
        )
    );
    if (res && res->modified_count() > 0) {
        (*client)[db_name]["book"].update_one(
            make_document(
                kvp("bookid", bookid),
                kvp("borrowRecord.userid", userid)
            ),
            make_document(
                kvp("$set", make_document(
                    kvp("borrowRecord.$.returnTime", returnTime)
                ))
            )
        );
        (*client)[db_name]["user"].update_one(
            make_document(
                kvp("userid", userid)
            ),
            make_document(
                kvp("$pull", make_document(
                    kvp("keep", make_document(
                        kvp("bookid", bookid)
                    ))
                ))
            )
        );
        (*client)[db_name]["user"].update_one(
            make_document(
                kvp("userid", userid),
                kvp("borrowRecord.bookid", bookid)
            ),
            make_document(
                kvp("$set", make_document(
                    kvp("borrowRecord.$.returnTime", returnTime)
                ))
            )
        );
        return X::NoError;
    } else
        return X::NoHave;
}

// maybe just use pt to search book not just by bookid
UserManager::ptree UserManager::getBookCore(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto priority = pt.get<xint>("priority");
    auto bookid = pt.get<xint>("bookid", 0);

    using namespace mongo;
    auto client = pool.acquire();
    mongocxx::options::find opt;
    opt.projection(
        make_document(
            kvp("_id", 0),
            kvp("keep", 0),
            kvp("borrowRecord", 0),
            kvp("resource", 0)
        )
    );
    auto doc = (*client)[db_name]["book"].find_one(
        make_document(
            kvp("bookid", bookid),
            kvp("priority", make_document(
                kvp("$lte", priority)
            ))
        ),
        opt
    );
    ptree p;
    if (doc)
        SocketInfo::decodePtree(bsoncxx::to_json(*doc, bsoncxx::ExtendedJsonMode::k_legacy), p);
    return std::move(p);
}

UserManager::ErrorCode UserManager::setBookCore(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto bookid = pt.get<xint>("bookid", 0);
    auto title = pt.get_optional<xstring>("title");
    auto author = pt.get_optional<xstring>("author");
    auto ISBN = pt.get_optional<xstring>("ISBN");
    auto publisher = pt.get_optional<xstring>("publisher");
    auto amount = pt.get_optional<xint>("amount");
    auto introduction = pt.get_optional<xstring>("introduction");
    auto position = pt.get_optional<xstring>("position");
    auto priority = pt.get_optional<xint>("priority");

    using namespace mongo;
    auto client = pool.acquire();
    auto ec = X::NoError;
    if (bookid) {
        auto info = (*client)[db_name]["book"].find_one(
            make_document(
                kvp("bookid", bookid)
            )
        );
        if (info && (title || author || ISBN || publisher || amount || introduction || position || priority)) {
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
                doc << "amount" << *amount;
            if (introduction)
                doc << "introduction" << bsoncxx::types::b_utf8(*introduction);
            if (position)
                doc << "position" << bsoncxx::types::b_utf8(*position);
            if (priority)
                doc << "priority" << *priority;
            doc << close_document;
            bsoncxx::document::value val = doc << finalize;
            (*client)[db_name]["book"].update_one(
                make_document(
                    kvp("bookid", bookid)
                ),
                val.view()
            );
        } else {
            ec = X::NoSuchBook;
        }
    } else {
        auto info = (*client)[db_name]["info"].find_one({});
        bookid = 1;
        if (info) {
            bookid = xint(info->view()["bookCount"].get_int32().value) + 1;
            (*client)[db_name]["info"].update_one(
                {},
                make_document(
                    kvp("$set", make_document(
                        kvp("bookCount", bookid)
                    ))
                )
            );
        } else {
            (*client)[db_name]["info"].insert_one(
                make_document(
                    kvp("bookCount", 1),
                    kvp("userCount", 0)
                )
            );
        }
        (*client)[db_name]["book"].insert_one(
            make_document(
                kvp("bookid", bookid),
                kvp("title", bsoncxx::types::b_utf8(title ? *title : "")),
                kvp("author", bsoncxx::types::b_utf8(author ? *author : "")),
                kvp("ISBN", bsoncxx::types::b_utf8(ISBN ? *ISBN : "")),
                kvp("publisher", bsoncxx::types::b_utf8(publisher ? *publisher : "")),
                kvp("amount", amount ? *amount : 0),
                kvp("introduction", bsoncxx::types::b_utf8(introduction ? *introduction : "")),
                kvp("position", bsoncxx::types::b_utf8(position ? *position : "")),
                kvp("priority", priority ? *priority : AbstractUser::SUPER_ADMINISTER),
                kvp("borrowRecord", make_array()),
                kvp("keep", make_array()),
                kvp("resource", make_array())
            )
        );
    }
    return ec;
}

UserManager::ptree UserManager::getRecord(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto type = pt.get<xstring>("type");
    auto userid = pt.get<xint>("userid");
    auto number = pt.get<xint>("number", 0);
    auto begin = pt.get<xint>("begin", 0);

    using namespace mongo;
    auto client = pool.acquire();

    mongocxx::options::find opt;
    opt.projection(
        make_document(
            kvp("_id", 0),
            kvp("userid", 1),
            kvp(type, make_document(
                kvp("$slice", -(begin + number))
            ))
        )
    );
    auto doc = (*client)[db_name]["user"].find_one(
        make_document(
            kvp("userid", userid)
        ),
        opt
    );
    ptree p;
    if (doc)
        SocketInfo::decodePtree(bsoncxx::to_json(*doc, bsoncxx::ExtendedJsonMode::k_legacy), p);
    return std::move(p);
}

// need: userid, ip, time, ensure the user is exist
void UserManager::recordLogin(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto userid = pt.get<xint>("userid");
    auto ip = pt.get<xstring>("ip");
    auto time = pt.get<xll>("time");

    using namespace mongo;
    auto client = pool.acquire();
    (*client)[db_name]["user"].update_one(
        make_document(
            kvp("userid", userid)
        ),
        make_document(
            kvp("$push", make_document(
                kvp("loginRecord", make_document(
                    kvp("ip", bsoncxx::types::b_utf8(ip)), 
                    kvp("time", time)
                ))
            ))
        )
    );
}

// need: userid, bookid, beginTime, endTime
void UserManager::recordBorrow(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto userid = pt.get<xint>("userid");
    auto bookid = pt.get<xint>("bookid");
    auto beginTime = pt.get<xll>("beginTime");
    auto endTime = pt.get<xll>("endTime");

    using namespace mongo;
    auto client = pool.acquire();
    (*client)[db_name]["user"].update_one(
        make_document(
            kvp("userid", userid)
        ),
        make_document(
            kvp("$push", make_document(
                kvp("borrowRecord", make_document(
                    kvp("bookid", bookid), 
                    kvp("beginTime", beginTime),
                    kvp("endTime", endTime)
                )),
                kvp("keep", make_document(
                    kvp("bookid", bookid),
                    kvp("beginTime", beginTime),
                    kvp("endTime", endTime)
                ))
            ))
        )
    );
    (*client)[db_name]["book"].update_one(
        make_document(
            kvp("bookid", userid)
        ),
        make_document(
            kvp("$push", make_document(
                kvp("borrowRecord", make_document(
                    kvp("userid", userid), 
                    kvp("beginTime", beginTime),
                    kvp("endTime", endTime)
                )),
                kvp("keep", make_document(
                    kvp("userid", userid),
                    kvp("beginTime", beginTime),
                    kvp("endTime", endTime)
                ))
            ))
        )
    );
}

// need: userid, bookid, time
void UserManager::recordBrowse(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto userid = pt.get<xint>("userid");
    auto bookid = pt.get<xint>("bookid");
    auto time = pt.get<xll>("time");

    using namespace mongo;
    auto client = pool.acquire();
    (*client)[db_name]["user"].update_one(
        make_document(
            kvp("userid", userid)
        ), 
        make_document(
            kvp("$push", make_document(
                kvp("browseRecord", make_document(
                    kvp("bookid", bookid), 
                    kvp("time", time)
                ))
            ))
        )
    );
}
