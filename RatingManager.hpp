#pragma once
#include <vector>
#include "Rating.hpp"
#include "BaseManager.hpp"

class RatingManager : public BaseManager {
private:
    std::vector<Rating> ratings;
    double totalRating;
    int ratingCount;
    double averageRating;
public:
    RatingManager() ;
    void addRating(const Rating& rating);
    int getRatingCount() const ;
    int size() const { return static_cast<int>(ratings.size()); }
    double getAverageRating() const ;
    void display() const;
    void loadFromfile() override;
    void saveToFile() override;
};