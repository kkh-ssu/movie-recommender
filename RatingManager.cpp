#include "Rating.hpp"
#include "RatingManager.hpp"
#include <iostream>
#include <sstream>
#include <fstream>


RatingManager::RatingManager() : totalRating(0.0), ratingCount(0), averageRating(0.0) {}

void RatingManager::addRating(const Rating& rating) {
    ratings.push_back(rating);
    totalRating += rating.getScore();
    ratingCount++;
    averageRating = totalRating / ratingCount;
}

int RatingManager::getRatingCount() const { return ratingCount; }
double RatingManager::getAverageRating() const { return averageRating; }

void RatingManager::display() const {
    for (const auto& r : ratings) {
        std::cout << "User: " << r.getUser().getName()
                  << ", Score: " << r.getScore() << std::endl;
    }
}

const std::vector<Rating>& RatingManager::getRatings() const {
    return ratings;
}
