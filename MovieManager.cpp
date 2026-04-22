#include <vector>
#include "Rating.hpp"
#include "Movie.hpp"
#include "MovieManager.hpp"
#include <iostream>
#include <algorithm>

MovieManager::MovieManager()  {}


int MovieManager::getMovieCount() const { return movies.size(); }

void MovieManager::addMovie(const Movie& movie) {
    movies.push_back(movie);
}

const Movie* MovieManager::findMovieByTitle(const std::string& title) const {
    for (const auto& movie : movies) {
        if (movie.getTitle() == title) {
            return &movie;
        }
    }
    return nullptr;
}

void MovieManager::displayMovies() const {
    std::cout << "Movies:" << std::endl;
    for (const auto& movie : movies) {
        std::cout << movie << std::endl;
    }
}

void MovieManager::displaySortedByRating() const {
    std::vector<Movie> sortedMovies = movies; 
    std::sort(sortedMovies.begin(), sortedMovies.end(),
              [](const Movie& a, const Movie& b) {
                  return a.getAverageRating() > b.getAverageRating(); 
              });
    
    std::cout << "평점 순으로 정렬된 영화 목록:" << std::endl;
    for (const auto& movie : sortedMovies) {
        std::cout << movie << " - Average Rating: " << movie.getAverageRating() << std::endl;
    }
}