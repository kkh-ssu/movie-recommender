#pragma once

#include "User.hpp"

class Rating{
    private:
        User user;
        int movieid;
        double score;
    public:
        Rating(const User& user, int movieid, double score);
        
        const User& getUser() const;
        int getMovieId() const;
        double getScore() const;



        friend std::ostream& operator<<(std::ostream& os, const Rating& r);

        
};