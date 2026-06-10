#include "UserManager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

UserManager::~UserManager() {}

void UserManager::addUser(const User& user) {
    users.push_back(user);
}

void UserManager::display() const {
    std::cout << "Users:" << std::endl;
    for (const auto& user : users) {
        std::cout << "ID: "     << user.getId()
                  << ", 이름: " << user.getName()
                  << ", 이메일: " << user.getEmail() << std::endl;
    }
}

bool UserManager::validUser(const std::string& username,
                            const std::string& email) const {
    for (const auto& user : users) {
        if (user.getName() == username && user.getEmail() == email)
            return true;
    }
    return false;
}

bool UserManager::validUser(const std::string& username) const {
    return findUserByName(username) != nullptr;
}

const User* UserManager::findUserByName(const std::string& username) const {
    for (const auto& user : users) {
        if (user.getName() == username) return &user;
    }
    return nullptr;
}

const User* UserManager::findUserById(int id) const {
    for (const auto& user : users) {
        if (user.getId() == id) return &user;
    }
    return nullptr;
}

int UserManager::getUserCount() const { return static_cast<int>(users.size()); }

const std::vector<User>& UserManager::getUsers() const { return users; }

void UserManager::loadFromfile() {
    std::ifstream userFile("data/users.csv");
    if (!userFile.is_open()) {
        std::cout << "users.csv를 열 수 없습니다." << std::endl;
        return;
    }

    std::string line;
    std::getline(userFile, line); // 헤더 스킵

    while (std::getline(userFile, line)) {
        std::istringstream ss(line);
        std::string idStr, name, email;
        std::getline(ss, idStr, '|');
        std::getline(ss, name,  '|');
        std::getline(ss, email, '|');

        try {
            int id = std::stoi(idStr);
            addUser(User(id, name, email));
        } catch (const std::invalid_argument&) {
            std::cout << "users.csv: 잘못된 형식의 줄을 건너뜁니다 → " << line << "\n";
        } catch (const std::out_of_range&) {
            std::cout << "users.csv: 범위를 벗어난 값이 있는 줄을 건너뜁니다 → " << line << "\n";
        }
    }
}

void UserManager::saveToFile() {
    std::ofstream userFile("data/users.csv");
    if (!userFile.is_open()) {
        std::cout << "users.csv를 열 수 없습니다." << std::endl;
        return;
    }

    userFile << "id|name|email\n";
    for (const auto& user : users) {
        userFile << user.getId()    << "|"
                 << user.getName()  << "|"
                 << user.getEmail() << "\n";
    }
}