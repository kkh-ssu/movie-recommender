#pragma once
#include <vector>
#include "Rating.hpp"
#include "UserManager.hpp"

class RatingManager {
private:
    std::vector<Rating> ratings;
    double totalRating;
    int ratingCount;
    double averageRating;
public:
    RatingManager();
    void addRating(const Rating& rating);
    int getRatingCount() const;
    const std::vector<Rating>& getRatings() const;
    void sortRatingsByUserId();

    int size() const;
    double getAverageRating() const;
    void display() const;
};