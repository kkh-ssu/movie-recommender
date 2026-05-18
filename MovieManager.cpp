#include <vector>
#include "Rating.hpp"
#include "Movie.hpp"
#include "MovieManager.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

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

Movie* MovieManager::findMovieByTitle(const std::string& title) {
    for (auto& movie : movies) {
        if (movie.getTitle() == title) {
            return &movie;
        }
    }
    return nullptr;
}

void MovieManager::display() const {
    std::cout << "영화 목록" << std::endl;
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

bool MovieManager::alreadyExists(const std::string& title,const std::string genre,int year) const {
    for (const auto& movie : movies) {
        if (movie.getTitle() == title && movie.getGenre() == genre && movie.getReleaseYear() == year) {
            return true;
        }
    }
    return false;
}

void MovieManager::loadFromfile() {
    std::ifstream movieFile("data/movies.csv");
    if (!movieFile.is_open()) {
        std::cout << "movie.csv를 열 수 없습니다." << std::endl;
        return;
    }
    std::string line;
    std::getline(movieFile, line);

    while(std::getline(movieFile, line)) {
        std::istringstream ss(line);
        std::string idStr, title, genre, yearStr;
        std::getline(ss, idStr, ',');
        std::getline(ss, title, ',');
        std::getline(ss, genre, ',');
        std::getline(ss, yearStr, ',');

        int id = std::stoi(idStr);
        int year = std::stoi(yearStr);
        addMovie(Movie(id, title, genre, year));
    }
}


void MovieManager::saveToFile() {
    std::ofstream movieFile("data/movies.csv");
    if (!movieFile.is_open()) {
        std::cout << "movie.csv를 열 수 없습니다." << std::endl;
        return;
    }
    movieFile << "id,title,genre,releaseYear\n";
    for (const auto& movie : movies) {
        movieFile << movie.getId() << ","
                  << movie.getTitle() << ","
                  << movie.getGenre() << ","
                  << movie.getReleaseYear() << "\n";
    }
}
