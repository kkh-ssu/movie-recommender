#include <vector>
#include "Rating.hpp"

class RatingManager {
    private:
        std::vector<Rating> ratings;
        double      totalRating;    
        int         ratingCount;   

    public:
        void addRating(const Rating& rating);
        double getAverageRating()   const;  

};