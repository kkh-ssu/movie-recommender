#include "User.hpp"
#include "UserManager.hpp"
#include <iostream>

void UserManager::addUser(const User& user) {
    users.push_back(user);
}

void UserManager::displayUsers() const {
    std::cout << "Users:" << std::endl;
    for (const auto& user : users) {
        std::cout << "ID: " << user.getId()
                  << ", 이름: " << user.getName()
                  << ", 이메일: " << user.getEmail() << std::endl;
    }
}

bool UserManager::validUser(const std::string& username, const std::string& email) const {
    for (const auto& user : users) {
        if (user.getName() == username && user.getEmail() == email) { return true; }
    }
    return false;
}

bool UserManager::validUser(const std::string &username) const{
    for (const auto& user : users) {
        if (user.getName() == username) { return true; }
    }
    return false;
}

int UserManager::getUserCount() const { return users.size(); }