#include "Rating.hpp"
#include <iostream>

Rating::Rating(const User& user, double score):user(user),score(score) { } //main에 validUser인지 체크하는 조건문 추가할것

const User& Rating::getUser() const {
    return user;
}

double Rating::getScore() const {
    return score;
}