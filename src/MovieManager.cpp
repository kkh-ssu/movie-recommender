#include "MovieManager.hpp"
#include "Constants.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
 
// getUserRatings() miss 시 반환할 빈 맵
static const std::unordered_map<int, double> EMPTY_RATINGS;
 
MovieManager::MovieManager()  {}
MovieManager::~MovieManager() {}
 
const std::vector<Movie>& MovieManager::getMovies() const { return movies; }
int MovieManager::getMovieCount() const { return static_cast<int>(movies.size()); }
 
// ── addMovie ──────────────────────────────────────────────────────────────────
// push_back 후 인덱스를 저장해 재할당 시에도 안전하게 O(1) 조회 가능
void MovieManager::addMovie(const Movie& movie) {
    movieIndex[movie.getId()] = movies.size();
    movies.push_back(movie);
}
 
// ── findMovieById ─────────────────────────────────────────────────────────────
// movieIndex로 O(1) 조회 (선형 탐색 제거)
Movie* MovieManager::findMovieById(int id) {
    auto it = movieIndex.find(id);
    return (it == movieIndex.end()) ? nullptr : &movies[it->second];
}
 
const Movie* MovieManager::findMovieById(int id) const {
    auto it = movieIndex.find(id);
    return (it == movieIndex.end()) ? nullptr : &movies[it->second];
}
 
// ── getUserRatings ────────────────────────────────────────────────────────────
// userRatingIndex로 O(1) 반환; 없으면 빈 맵 반환
const std::unordered_map<int, double>& MovieManager::getUserRatings(int userId) const {
    auto it = userRatingIndex.find(userId);
    return (it == userRatingIndex.end()) ? EMPTY_RATINGS : it->second;
}
 
// ── getUserRatingCount ────────────────────────────────────────────────────────
// userRatingIndex로 O(1) 조회 (이중 루프 제거)
int MovieManager::getUserRatingCount(int userId) const {
    auto it = userRatingIndex.find(userId);
    return (it == userRatingIndex.end()) ? 0 : static_cast<int>(it->second.size());
}
 
// ── loadRatings ───────────────────────────────────────────────────────────────
// 평점 추가와 동시에 userRatingIndex를 구축해 이후 O(1) 조회 가능하게 함
void MovieManager::loadRatings(UserManager& userManager) {
    std::ifstream file("data/ratings.csv");
    if (!file.is_open()) {
        std::cout << "ratings.csv를 열 수 없습니다." << std::endl;
        return;
    }
 
    std::string line;
    std::getline(file, line); // 헤더 스킵
 
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string userIdStr, movieIdStr, scoreStr;
        std::getline(ss, userIdStr,  '|');
        std::getline(ss, movieIdStr, '|');
        std::getline(ss, scoreStr,   '|');
 
        // 파일 손상에 대비해 변환 예외 처리
        try {
            int    userId  = std::stoi(userIdStr);
            int    movieId = std::stoi(movieIdStr);
            double score   = std::stod(scoreStr);
 
            Movie*      movie = findMovieById(movieId);
            const User* user  = userManager.findUserById(userId);
 
            if (movie && user) {
                movie->getRatingManager().addRating(Rating(*user, score));
                userRatingIndex[userId][movieId] = score;
            }
        } catch (const std::invalid_argument&) {
            std::cout << "ratings.csv: 잘못된 형식의 줄을 건너뜁니다 → " << line << "\n";
        } catch (const std::out_of_range&) {
            std::cout << "ratings.csv: 범위를 벗어난 값이 있는 줄을 건너뜁니다 → " << line << "\n";
        }
    }
}
 
// ── saveRatings ───────────────────────────────────────────────────────────────
// score의 정확성을 위해 RatingManager 데이터를 직접 순회
void MovieManager::saveRatings() {
    std::ofstream file("data/ratings.csv");
    if (!file.is_open()) {
        std::cout << "ratings.csv를 열 수 없습니다." << std::endl;
        return;
    }
 
    file << "userId|movieId|score\n";
    for (const auto& movie : movies) {
        for (const auto& r : movie.getRatingManager().getRatings()) {
            file << r.getUser().getId() << "|"
                 << movie.getId()       << "|"
                 << r.getScore()        << "\n";
        }
    }
}
 
const Movie* MovieManager::findMovieByTitle(const std::string& title) const {
    for (const auto& movie : movies) {
        if (movie.getTitle() == title) return &movie;
    }
    return nullptr;
}
 
Movie* MovieManager::findMovieByTitle(const std::string& title) {
    for (auto& movie : movies) {
        if (movie.getTitle() == title) return &movie;
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
    // 내림차순 정렬: operator> 활용
    std::sort(sortedMovies.begin(), sortedMovies.end(),
              [](const Movie& a, const Movie& b) { return a > b; });
 
    std::cout << "평점 순으로 정렬된 영화 목록:" << std::endl;
    for (const auto& movie : sortedMovies) {
        std::cout << movie
                  << " - Average Rating: "
                  << std::fixed << std::setprecision(RATING_PRECISION)
                  << movie.getAverageRating() << std::endl;
    }
}
 
bool MovieManager::alreadyExists(const std::string& title,
                                 const std::string& genre, int year) const {
    for (const auto& movie : movies) {
        if (movie.getTitle()       == title &&
            movie.getGenre()       == genre &&
            movie.getReleaseYear() == year)
            return true;
    }
    return false;
}
 
void MovieManager::loadFromfile() {
    std::ifstream movieFile("data/movies.csv");
    if (!movieFile.is_open()) {
        std::cout << "movies.csv를 열 수 없습니다." << std::endl;
        return;
    }
 
    std::string line;
    std::getline(movieFile, line); // 헤더 스킵
 
    while (std::getline(movieFile, line)) {
        std::istringstream ss(line);
        std::string idStr, title, genre, yearStr;
        std::getline(ss, idStr,   '|');
        std::getline(ss, title,   '|');
        std::getline(ss, genre,   '|');
        std::getline(ss, yearStr, '|');
 
        try {
            int id   = std::stoi(idStr);
            int year = std::stoi(yearStr);
            addMovie(Movie(id, title, genre, year));
        } catch (const std::invalid_argument&) {
            std::cout << "movies.csv: 잘못된 형식의 줄을 건너뜁니다 → " << line << "\n";
        } catch (const std::out_of_range&) {
            std::cout << "movies.csv: 범위를 벗어난 값이 있는 줄을 건너뜁니다 → " << line << "\n";
        }
    }
}
 
void MovieManager::saveToFile() {
    std::ofstream movieFile("data/movies.csv");
    if (!movieFile.is_open()) {
        std::cout << "movies.csv를 열 수 없습니다." << std::endl;
        return;
    }
 
    movieFile << "id|title|genre|releaseYear\n";
    for (const auto& movie : movies) {
        movieFile << movie.getId()          << "|"
                  << movie.getTitle()       << "|"
                  << movie.getGenre()       << "|"
                  << movie.getReleaseYear() << "\n";
    }
}
