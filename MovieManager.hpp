#pragma once

#include <vector>
#include "Movie.hpp"

class MovieManager {
private:
    std::vector<Movie> movies;
public:
    MovieManager();
    void addMovie(const Movie& movie);
    void findMovieByTitle(const std::string& title) const;  
    void displayMovies() const;
    void displaySortedMovies() const; 
    int getMovieCount() const;
};