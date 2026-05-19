#pragma once
#include <vector>
#include "BaseManager.hpp"
#include "User.hpp"
#include <string>

class UserManager : public BaseManager {
    private:
        std::vector<User> users;

    public:
        ~UserManager();
        void addUser(const User& user);
        void display() const;
        int getUserCount() const ;
        const std::vector<User>& getUsers() const;
        int size() const { return static_cast<int>(users.size()); }
        bool validUser(const std::string& username,const std::string& email) const;
        bool validUser(const std::string& username) const;
        const User* findUserByName(const std::string& username) const;
        const User* findUserById(int id) const;
        void loadFromfile() override;
        void saveToFile() override;
};