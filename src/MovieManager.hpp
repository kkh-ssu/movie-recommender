#pragma once
 
#include <vector>
#include <unordered_map>
#include "BaseManager.hpp"
#include "UserManager.hpp"
#include "Movie.hpp"
 
class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;
 
    // 역색인 1: movieId → index in movies vector
    std::unordered_map<int, size_t> movieIndex;
 
    // 역색인 2: userId → { movieId → score }
    std::unordered_map<int, std::unordered_map<int, double>> userRatingIndex;
 
public:
    MovieManager();
    ~MovieManager();
 
    const std::vector<Movie>& getMovies() const;
    void addMovie(const Movie& movie);
 
    const Movie* findMovieByTitle(const std::string& title) const;
    Movie*       findMovieByTitle(const std::string& title);
    Movie*       findMovieById(int id);
    const Movie* findMovieById(int id) const;
 
    // 역색인 조회: userId의 { movieId → score } 맵 반환
    const std::unordered_map<int, double>& getUserRatings(int userId) const;
 
    // userRatingIndex 갱신 (평점 수정 시 MenuHandler 에서 호출)
    void updateUserRatingIndex(int userId, int movieId, double newScore);
 
    void display()               const;
    void displaySortedByRating() const;
    int  getMovieCount()         const;
    int  getUserRatingCount(int userId) const;
    int  size() const { return static_cast<int>(movies.size()); }
    bool alreadyExists(const std::string& title,
                       const std::string& genre, int year) const;
 
    void loadFromfile() override;
    void saveToFile()   override;
    void loadRatings(UserManager& userManager);
    void saveRatings();
};
 