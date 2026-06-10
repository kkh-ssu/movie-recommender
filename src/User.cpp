#include "User.hpp"
#include <iostream>

User::User(int id, const std::string& name, const std::string& email)
    : id(id), name(name), email(email) {}

int                User::getId()    const { return id; }
const std::string& User::getName()  const { return name; }
const std::string& User::getEmail() const { return email; }

std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "id: "    << u.id
       << " 이름: " << u.name
       << " 이메일: " << u.email;
    return os;
}