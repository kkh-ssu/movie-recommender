#pragma once
#include <vector>
#include "Rating.hpp"

class RatingManager {
private:
    std::vector<Rating> ratings;
    double totalRating;
    int ratingCount;
    double averageRating;
public:
    RatingManager() ;
    void addRating(const Rating& rating);
    int getRatingCount() const ;
    double getAverageRating() const ;
    void displayRatings() const;
};