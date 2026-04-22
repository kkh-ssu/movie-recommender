#include <vector>
#include <iostream>
#include "Rating.hpp"

class RatingManager {
    private:
        std::vector<Rating> ratings;
        double      totalRating;    
        int         ratingCount;   
        double      averageRating;

    public:
        RatingManager() : totalRating(0.0), ratingCount(0),averageRating(0.0) {}
        void addRating(const Rating& rating){
            ratings.push_back(rating);
            totalRating += rating.getScore();
            ratingCount++;
            averageRating = totalRating / ratingCount;
        }
        int getRatingCount() const { return ratingCount; }
        double getAverageRating() const { return averageRating; }
        void displayRatings() {
            std::cout << "Ratings:" << std::endl;
            for (const auto& rating : ratings) {
                std::cout << "User: " << rating.getUser().getName()
                          << ", Movie ID: " << rating.getMovieId()
                          << ", Score: " << rating.getScore() << std::endl;
            }
        }
};