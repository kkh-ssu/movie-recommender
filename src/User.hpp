#pragma once
#include <string>

class User {
private:
    int         id;
    std::string name;
    std::string email;

public:
    // const& 로 받아 불필요한 복사 방지
    User(int id, const std::string& name, const std::string& email);
    ~User() = default;

    // 큰 객체(string)는 const& 로 반환
    int                getId()    const;
    const std::string& getName()  const;
    const std::string& getEmail() const;

    friend std::ostream& operator<<(std::ostream& os, const User& u);
};