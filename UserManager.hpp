#include <vector>
#include "User.hpp"
#include <string>

class UserManager {
    private:
        std::vector<User> users;

    public:
        void addUser(const User& user);
        void displayUsers() const;
        int getUserCount() const ;
        bool validUser(const std::string& username,const std::string& email) const;
        bool validUser(const std::string& username) const;
};