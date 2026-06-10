#include "Rating.hpp"
#include <iostream>
 
Rating::Rating(const User& user, double score) : user(user), score(score) {}
 
const User& Rating::getUser() const { return user; }
double Rating::getScore() const { return score; }
void   Rating::setScore(double newScore) { score = newScore; }