#include "usermanager.h"

UserManager::UserManager(const X::string &sql_url, const int &sql_port) {

}

UserManager::~UserManager() {

}

bool UserManager::isUser(const X::uint &userid) const {
    return true;
}

int UserManager::getPriority(const int &userid) const {
    return 100;
}

void UserManager::registerUser(const X::string &username, const X::string &password, const int &priority) {

}
