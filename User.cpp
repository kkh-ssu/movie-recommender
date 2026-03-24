#include "User.hpp"
#include <iostream>

User::User(int id, const std::string name,
    const std::string email):id(id),name(name),email(email) { }

User::~User() { }

int User::getId() const{
    return id;
}

std::string User::getName() const{
    return name;
}

std::string User::getEmail() const{
    return email;
}

void User::display() const {
    std::cout<<"id: "<<id<<std::endl<<
    "name: "<<name<<std::endl<<
    "email: "<<email<<std::endl;
}
