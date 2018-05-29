// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <algorithm>
#include <regex>

#include <core/SocketInfo.h>
#include <server/manager/UserManager.h>
#include <server/utils.h>

UserManager::UserManager(const xstring &mongo_url, const xstring &mongo_db_name) :
    pool(mongo::uri(mongo_url)),
    _db_name(mongo_db_name),
    db_name(_db_name.c_str()) {

}

bool UserManager::isDBEmpty() {
    using namespace mongo;
    auto client = pool.acquire();
    auto info = (*client)[db_name]["info"].find_one({});
    return !info;
}

bool UserManager::isUser(const xint &userid) {
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
            kvp("starRecord", 0),
            kvp("borrowRecord", 0),
            kvp("keepRecord", 0),
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

UserManager::ErrorCode UserManager::registerUser(const ptree &pt, int priority) {
    cerr << SocketInfo::encodePtree(pt, true);
    xstring username = pt.get("username", "");
    xstring nickname = pt.get("nickname", "");
    xstring password = pt.get("password", "");
    xstring email = pt.get("email", "");

    if (!X::checkUsername(username))
        return X::InvalidUsername;
    if (!X::checkNickname(nickname))
        return X::InvalidNickname;
    if (!X::checkPassword(password))
        return X::InvalidPassword;
    if (!X::checkEmail(email))
        return X::InvalidEmail;
    if (isUser(username))
        return X::AlreadyRegister;

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
            kvp("priority", priority),
            kvp("loginRecord", make_array()),
            kvp("starRecord", make_array()),
            kvp("borrowRecord", make_array()),
            kvp("keepRecord", make_array()),
            kvp("browseRecord", make_array())
        )
    );

    return X::NoError;
}

UserManager::ErrorCode UserManager::modifyUser(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto userid = pt.get<xint>("userid");
    auto nickname = pt.get_optional<xstring>("nickname");
    auto email = pt.get_optional<xstring>("email");
    auto passwordOld = pt.get_optional<xstring>("passwordOld");
    auto passwordNew = pt.get_optional<xstring>("passwordNew");

    if (!nickname || !X::checkNickname(*nickname))
        return X::InvalidNickname;
    if (!email || !X::checkEmail(*email))
        return X::InvalidEmail;
    if (!passwordOld)
        return X::InvalidPassword;
    if (passwordNew && !X::checkPassword(*passwordNew))
        return X::InvalidNewPassword;

    using namespace mongo;
    auto client = pool.acquire();
    auto info = (*client)[db_name]["user"].find_one(
        make_document(
            kvp("userid", userid),
            kvp("password", bsoncxx::types::b_utf8(*passwordOld))
        )
    );
    if (!info)
        return X::InvalidPassword;

    auto doc = document();
    doc << "$set" << open_document;
    doc << "nickname" << bsoncxx::types::b_utf8(*nickname);
    doc << "email" << bsoncxx::types::b_utf8(*email);
    if (passwordNew)
        doc << "password" << bsoncxx::types::b_utf8(*passwordNew);
    doc << close_document;
    bsoncxx::document::value val = doc << finalize;
    (*client)[db_name]["user"].update_one(
        make_document(
            kvp("userid", userid)
        ),
        val.view()
    );

    return X::NoError;
}

// maybe need to verify time
UserManager::ErrorCode UserManager::borrowBook(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto userid = pt.get<xint>("userid");
    auto priority = pt.get<xint>("priority");
    auto bookid = pt.get<xint>("bookid", 0);
    auto t = pt.get<xll>("keepTime", 0);
    auto beginTime = pt.get<xll>("beginTime", 0);
    auto keepTime = pt.get<xll>("keepTime", 0);
    auto endTime = beginTime + keepTime;

    if (!bookid)
        return X::NoSuchBook;

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
            kvp("maxKeepTime", 1),
            kvp("count", make_document(
                kvp("$size", "$keepRecord")
            ))
        )
    );
    auto cur = (*client)[db_name]["book"].aggregate(p);
    ErrorCode ec = X::NoSuchBook;
    if (cur.begin() != cur.end()) {
        auto view = *cur.begin();
        auto amount = xint(view["amount"].get_int32().value);
        auto maxKeepTime = xll(view["maxKeepTime"].get_int64().value);
        auto count = xint(view["count"].get_int32().value);
        if (count + 1 > amount) {
            ec = X::NoRestBook;
        } else if (keepTime <= 0 || keepTime > maxKeepTime) {
            ec = X::InvalidTime;
        } else {
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
                    kvp("keepRecord.userid", userid)
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
                            kvp("keepRecord", make_document(
                                kvp("bookid", bookid),
                                kvp("beginTime", beginTime),
                                kvp("endTime", endTime)
                            )),
                            kvp("borrowRecord", make_document(
                                kvp("bookid", bookid),
                                kvp("beginTime", beginTime),
                                kvp("endTime", endTime),
                                kvp("returnTime", xll(0))
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
                            kvp("keepRecord", make_document(
                                kvp("userid", userid),
                                kvp("beginTime", beginTime),
                                kvp("endTime", endTime)
                            )),
                            kvp("borrowRecord", make_document(
                                kvp("userid", userid),
                                kvp("beginTime", beginTime),
                                kvp("endTime", endTime),
                                kvp("returnTime", xll(0))
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

    if (!bookid)
        return X::NoSuchBook;
    if (!userid)
        return X::NoSuchUser;

    using namespace mongo;
    auto client = pool.acquire();
    auto res = (*client)[db_name]["book"].update_one(
        make_document(
            kvp("bookid", bookid)
        ),
        make_document(
            kvp("$pull", make_document(
                kvp("keepRecord", make_document(
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
                    kvp("keepRecord", make_document(
                        kvp("bookid", bookid)
                    ))
                ))
            )
        );
        (*client)[db_name]["user"].update_one(
            make_document(
                kvp("userid", userid),
                kvp("borrowRecord.bookid", bookid),
                kvp("borrowRecord.returnTime", xll(0))
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

UserManager::ErrorCode UserManager::starBook(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto userid = pt.get<xint>("userid");
    auto priority = pt.get<xint>("priority");
    auto time = pt.get<xll>("time");
    auto bookid = pt.get<xint>("bookid", 0);

    if (!bookid)
        return X::NoSuchBook;

    using namespace mongo;
    auto client = pool.acquire();

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
            kvp("starRecord.userid", userid)
        )
    );
    if (doc)
        return X::AlreadyStar;

    auto res = (*client)[db_name]["book"].update_one(
        make_document(
            kvp("bookid", bookid),
            kvp("priority", make_document(
                kvp("$lte", priority)
            ))
        ),
        make_document(
            kvp("$inc", make_document(
                kvp("starCount", 1)
            )),
            kvp("$push", make_document(
                kvp("starRecord", make_document(
                    kvp("userid", userid),
                    kvp("time", time)
                ))
            ))
        )
    );
    if (res && res->modified_count() > 0) {
        (*client)[db_name]["user"].update_one(
            make_document(
                kvp("userid", userid)
            ),
            make_document(
                kvp("$push", make_document(
                    kvp("starRecord", make_document(
                        kvp("bookid", bookid),
                        kvp("time", time)
                    ))
                ))
            )
        );
        return X::NoError;
    }
    return X::NoSuchBook;
}

UserManager::ErrorCode UserManager::unStarBook(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto userid = pt.get<xint>("userid");
    auto bookid = pt.get<xint>("bookid", 0);

    if (!bookid)
        return X::NoSuchBook;

    using namespace mongo;
    auto client = pool.acquire();
    auto res = (*client)[db_name]["book"].update_one(
        make_document(
            kvp("bookid", bookid)
        ),
        make_document(
            kvp("$pull", make_document(
                kvp("starRecord", make_document(
                    kvp("userid", userid)
                ))
            ))
        )
    );
    if (res && res->modified_count() > 0) {
        (*client)[db_name]["book"].update_one(
            make_document(
                kvp("bookid", bookid)
            ),
            make_document(
                kvp("$inc", make_document(
                    kvp("starCount", -1)
                ))
            )
        );
        (*client)[db_name]["user"].update_one(
            make_document(
                kvp("userid", userid)
            ),
            make_document(
                kvp("$pull", make_document(
                    kvp("starRecord", make_document(
                        kvp("bookid", bookid)
                    ))
                ))
            )
        );
        return X::NoError;
    }
    return X::NoSuchStar;
}

// maybe just use pt to search book not just by bookid
UserManager::ptree UserManager::getBook(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto userid = pt.get<xint>("userid");
    auto priority = pt.get<xint>("priority");
    auto time = pt.get<xll>("time");
    auto bookid = pt.get<xint>("bookid", 0);

    if (!bookid)
        return {};

    using namespace mongo;
    auto client = pool.acquire();
    mongocxx::options::find opt;
    opt.projection(
        make_document(
            kvp("_id", 0),
            kvp("starRecord", 0),
            kvp("keepRecord", 0),
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
    if (doc) {
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
        SocketInfo::decodePtree(bsoncxx::to_json(*doc, bsoncxx::ExtendedJsonMode::k_legacy), p);
    }
    return std::move(p);
}

UserManager::ptree UserManager::getBookBrief(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto userid = pt.get<xint>("userid");
    auto priority = pt.get<xint>("priority");
    auto bookid = pt.get<xint>("bookid", 0);

    if (!bookid)
        return {};

    using namespace mongo;
    auto client = pool.acquire();
    mongocxx::options::find opt;
    opt.projection(
        make_document(
            kvp("_id", 0),
            kvp("bookid", 1),
            kvp("title", 1),
            kvp("author", 1),
            kvp("ISBN", 1),
            kvp("publisher", 1),
            kvp("introduction", 1),
            kvp("maxKeepTime", 1),
            kvp("priority", 1),
            kvp("starCount", 1)
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

UserManager::ErrorCode UserManager::setBook(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto userPriority = pt.get<xint>("userPriority");
    auto bookid = pt.get<xint>("bookid", 0);
    auto title = pt.get_optional<xstring>("title");
    auto author = pt.get_optional<xstring>("author");
    auto ISBN = pt.get_optional<xstring>("ISBN");
    auto publisher = pt.get_optional<xstring>("publisher");
    auto amount = pt.get_optional<xint>("amount");
    auto introduction = pt.get_optional<xstring>("introduction");
    auto position = pt.get_optional<xstring>("position");
    auto priority = pt.get_optional<xint>("priority");
    auto maxKeepTime = pt.get_optional<xll>("maxKeepTime");

    using namespace mongo;
    auto client = pool.acquire();
    auto ec = X::NoError;
    if (bookid) {
        auto info = (*client)[db_name]["book"].find_one(
            make_document(
                kvp("bookid", bookid),
                kvp("priority", make_document(
                    kvp("$lte", userPriority)
                ))
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
            if (maxKeepTime)
                doc << "maxKeepTime" << *maxKeepTime;
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
                kvp("priority", priority ? *priority : int(X::ADMINISTER)),
                kvp("maxKeepTime", maxKeepTime ? *maxKeepTime : 0),
                kvp("starCount", 0),
                kvp("starRecord", make_array()),
                kvp("borrowRecord", make_array()),
                kvp("keepRecord", make_array()),
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

UserManager::ptree UserManager::getNewBookList(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto priority = pt.get<xint>("priority");
    auto number = std::min(50, pt.get<xint>("number", 0));

    using namespace mongo;

    auto client = pool.acquire();

    mongocxx::options::find opt;

    auto count = (*client)[db_name]["book"].count(
        make_document(
            kvp("priority", make_document(
                kvp("$lte", priority)
            ))
        )
    );
    opt.projection(
        make_document(
            kvp("_id", 0),
            kvp("bookid", 1)
        )
    ).skip(std::max(0ll, count - number));
    auto cur = (*client)[db_name]["book"].find(
        make_document(
            kvp("priority", make_document(
                kvp("$lte", priority)
            ))
        ),
        opt
    );
    ptree p;
    ptree child;
    ptree value;
    for (auto &&doc : cur) {
        auto bookid = xint(doc["bookid"].get_int32().value);
        value.put("", bookid);
        child.push_back(std::make_pair("", value));
    }
    p.add_child("bookid", child);
    return std::move(p);
}

UserManager::xstring UserManager::getResource(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto priority = pt.get<xint>("priority");
    auto resourceName = pt.get<xstring>("resourceName");
    auto bookid = pt.get<xint>("bookid", 0);
    xstring path = "";

    if (!bookid)
        return "";

    using namespace mongo;
    auto client = pool.acquire();
    mongocxx::options::find opt;
    opt.projection(
        make_document(
            kvp("_id", 0),
            kvp("resource.$", 1)
        )
    );
    auto doc = (*client)[db_name]["book"].find_one(
        make_document(
            kvp("bookid", bookid),
            kvp("priority", make_document(
                kvp("$lte", priority)
            )),
            kvp("resource.name", resourceName)
        ),
        opt
    );

    if (doc)
        path = xstring(doc->view()["resource"].get_array().value.begin()->get_document().value["path"].get_utf8().value);

    return path;
}

UserManager::ErrorCode UserManager::setResource(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto priority = pt.get<xint>("priority");
    auto resourceName = pt.get<xstring>("resourceName");
    auto resourcePath = pt.get<xstring>("resourcePath");
    auto bookid = pt.get<xint>("bookid", 0);

    if (!bookid)
        return X::NoSuchBook;

    using namespace mongo;
    auto client = pool.acquire();
    mongocxx::options::find opt;
    opt.projection(
        make_document(
            kvp("_id", 0),
            kvp("resource", 1)
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
    if (!doc)
        return X::NoSuchBook;
    (*client)[db_name]["book"].update_one(
        make_document(
            kvp("bookid", bookid)
        ),
        make_document(
            kvp("$pull", make_document(
                kvp("resource", make_document(
                    kvp("name", resourceName)
                ))
            ))
        )
    );
    auto res = (*client)[db_name]["book"].update_one(
        make_document(
            kvp("bookid", bookid)
        ),
        make_document(
            kvp("$push", make_document(
                kvp("resource", make_document(
                    kvp("name", resourceName),
                    kvp("path", resourcePath)
                ))
            ))
        )
    );
    if (res->modified_count() != 1)
        return X::InvalidResource;
    return X::NoError;
}

UserManager::ptree UserManager::getSearchBookList(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto priority = pt.get<xint>("priority");
    auto bookid = pt.get_optional<xstring>("bookid");
    auto title = pt.get_optional<xstring>("title");
    auto author = pt.get_optional<xstring>("author");
    auto ISBN = pt.get_optional<xstring>("ISBN");
    auto publisher = pt.get_optional<xstring>("publisher");
    auto introduction = pt.get_optional<xstring>("introduction");
    auto position = pt.get_optional<xstring>("position");

    using namespace mongo;
    auto client = pool.acquire();

    auto doc = document();
    if (bookid)
        doc << "bookid" << open_document << "$regex" << bsoncxx::types::b_utf8(*bookid) << close_document;
    if (title)
        doc << "title" << open_document << "$regex" << bsoncxx::types::b_utf8(*title) << close_document;
    if (author)
        doc << "author" << open_document << "$regex" << bsoncxx::types::b_utf8(*author) << close_document;
    if (ISBN)
        doc << "ISBN" << open_document << "$regex" << bsoncxx::types::b_utf8(*ISBN) << close_document;
    if (publisher)
        doc << "publisher" << open_document << "$regex" << bsoncxx::types::b_utf8(*publisher) << close_document;
    if (introduction)
        doc << "introduction" << open_document << "$regex" << bsoncxx::types::b_utf8(*introduction) << close_document;
    if (position)
        doc << "position" << open_document << "$regex" << bsoncxx::types::b_utf8(*position) << close_document;
    bsoncxx::document::value val = doc << finalize;

    mongocxx::options::find opt;
    opt.projection(
        make_document(
            kvp("_id", 0),
            kvp("bookid", 1)
        )
    );

    auto cur = (*client)[db_name]["book"].find(
        val.view(),
        opt
    );

    ptree p;
    ptree child;
    ptree value;
    for (auto &&doc : cur) {
        auto bookid = xint(doc["bookid"].get_int32().value);
        value.put("", bookid);
        child.push_back(std::make_pair("", value));
    }
    p.add_child("bookid", child);
    return std::move(p);
}

UserManager::ErrorCode UserManager::setPriority(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    xint userPriority = pt.get<xint>("userPriority");
    xint userid = pt.get<xint>("userid", 0);
    xint priority = pt.get<xint>("priority", X::USER);
    if (priority >= userPriority)
        return X::NoPermission;

    using namespace mongo;
    auto client = pool.acquire();
    auto res = (*client)[db_name]["user"].update_one(
        make_document(
            kvp("userid", userid),
            kvp("priority", make_document(
                kvp("$lt", userPriority)
            ))
        ),
        make_document(
            kvp("$set", make_document(
                kvp("priority", priority)
            ))
        )
    );
    if (res->matched_count() != 1)
        return X::NoSuchUser;
    if (res->matched_count() != 1)
        return X::NoPermission;
    return X::NoError;
}

UserManager::ptree UserManager::getTopBookList(const ptree &pt) {
    cerr << SocketInfo::encodePtree(pt, true);
    auto priority = pt.get<xint>("priority");
    auto number = std::min(50, pt.get<xint>("number", 0));

    using namespace mongo;

    auto client = pool.acquire();

    mongocxx::options::find opt;

    opt.projection(
        make_document(
            kvp("_id", 0),
            kvp("bookid", 1)
        )
    ).sort(make_document(kvp("starCount", -1))).limit(number);
    auto cur = (*client)[db_name]["book"].find(
        make_document(
            kvp("priority", make_document(
                kvp("$lte", priority)
            ))
        ),
        opt
    );
    ptree p;
    ptree child;
    ptree value;
    for (auto &&doc : cur) {
        auto bookid = xint(doc["bookid"].get_int32().value);
        value.put("", bookid);
        child.push_back(std::make_pair("", value));
    }
    p.add_child("bookid", child);
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
