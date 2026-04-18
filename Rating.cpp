#include "Rating.hpp"
#include <iostream>

Rating::Rating(const User& user, int movieid, double score):user(user),
movieid(movieid),score(score) { } //main에 validUser인지 체크하는 조건문 추가할것

const User& Rating::getUser() const {
    return user;
}

int Rating::getMovieId() const {
    return movieid;
}

double Rating::getScore() const {
    return score;
}

