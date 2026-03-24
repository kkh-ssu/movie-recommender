#include "Rating.hpp"
#include <iostream>

Rating::Rating(int userid, int movieid, double score):userid(userid),
movieid(movieid),score(score) { }

int Rating::getUserid() const {
    return userid;
}

int Rating::movieId() const {
    return movieid;
}

double Rating::getScore() const {
    return score;
}

void Rating::display() const {
    std::cout<<"user ID: "<<userid<<std::endl<<
    "movie ID: "<<movieid<<std::endl<<
    "score: "<<score<<std::endl;
}
