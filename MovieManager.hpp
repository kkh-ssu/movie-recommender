#pragma once

#include <vector>
#include "Movie.hpp"

class MovieManager {
private:
    std::vector<Movie> movies;
public:
    MovieManager();
    void addMovie(const Movie& movie);
    const Movie* findMovieByTitle(const std::string& title) const; 
    Movie* findMovieByTitle(const std::string& title);    
    void displayMovies() const;
    void displaySortedByRating() const; 
    int getMovieCount() const;
};