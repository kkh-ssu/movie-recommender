#pragma once

#include "MovieManager.hpp"
#include "UserManager.hpp"
#include "RatingManager.hpp"

class Recommender {
    private:
        MovieManager& movieManager;
        UserManager& userManager;
        RatingManager allRatings;
    public:
        Recommender(MovieManager& mm, UserManager& um, RatingManager& ar) : 
        movieManager(mm), userManager(um), allRatings(ar) {}
        void loadRatings();
        int similaritycalc(const User& mainUser);
        void recommendMovies(const User& mainUser);
};