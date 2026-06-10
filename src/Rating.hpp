#pragma once
 
#include "User.hpp"
 
class Rating {
    private:
        User user;
        double score;
    public:
        Rating(const User& user, double score);
        
        const User& getUser() const;
        double getScore() const;
        void   setScore(double newScore); // updateRating 에서 필요
 
        friend std::ostream& operator<<(std::ostream& os, const Rating& r);
};
 