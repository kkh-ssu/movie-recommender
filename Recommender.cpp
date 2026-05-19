#include "Recommender.hpp"

void Recommender::loadRatings() {
    for (const auto& movie : movieManager.getMovies()) {
        const auto& ratings = movie.getRatingManager().getRatings();
        for (const auto& rating : ratings) {
            allRatings.addRating(rating);
        }
    }
    allRatings.sortRatingsByUserId();
}

int Recommender::similaritycalc(const User &mainUser) {
    return 0;
}
