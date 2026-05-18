#pragma once

#include <vector>
#include "BaseManager.hpp"
#include "Movie.hpp"

class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;
public:
    MovieManager();
    void addMovie(const Movie& movie);
    const Movie* findMovieByTitle(const std::string& title) const; 
    Movie* findMovieByTitle(const std::string& title);  
    Movie* findMovieById(int id);
    void display() const;
    void displaySortedByRating() const; 
    int getMovieCount() const;
    int size() const { return static_cast<int>(movies.size()); }
    bool alreadyExists(const std::string& title,const std::string genre,int year) const;
    void loadFromfile() override;
    void saveToFile() override;
    void loadRatings(UserManager& userManager);
    void saveRatings();
};