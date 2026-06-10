#include <vector>
#include "Rating.hpp"
#include "Movie.hpp"
#include "MovieManager.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>

// 빈 맵 — getUserRatings() miss 시 반환용
static const std::unordered_map<int, double> EMPTY_RATINGS;

MovieManager::MovieManager()  {}
MovieManager::~MovieManager() {}

const std::vector<Movie>& MovieManager::getMovies() const {
    return movies;
}

int MovieManager::getMovieCount() const { return movies.size(); }

// ── addMovie ─────────────────────────────────────────────────────────────────
// movies에 추가하는 동시에 movieIndex를 갱신한다.
// push_back 후 인덱스를 저장하므로 재할당 시에도 포인터 대신 인덱스를 쓰기에 안전.
void MovieManager::addMovie(const Movie& movie) {
    movieIndex[movie.getId()] = movies.size();
    movies.push_back(movie);
}

// ── findMovieById ─────────────────────────────────────────────────────────────
// 기존: O(M) 선형 탐색 → 개선: O(1) 인덱스 조회
Movie* MovieManager::findMovieById(int id) {
    auto it = movieIndex.find(id);
    if (it == movieIndex.end()) return nullptr;
    return &movies[it->second];
}

const Movie* MovieManager::findMovieById(int id) const {
    auto it = movieIndex.find(id);
    if (it == movieIndex.end()) return nullptr;
    return &movies[it->second];
}

// ── getUserRatings ────────────────────────────────────────────────────────────
// 역색인에서 userId의 { movieId → score } 맵을 O(1)로 반환.
// 없으면 빈 맵 반환.
const std::unordered_map<int, double>&
MovieManager::getUserRatings(int userId) const {
    auto it = userRatingIndex.find(userId);
    if (it == userRatingIndex.end()) return EMPTY_RATINGS;
    return it->second;
}

// ── getUserRatingCount ────────────────────────────────────────────────────────
// 기존: O(M×R) 이중 루프 → 개선: O(1) 인덱스 조회
int MovieManager::getUserRatingCount(int userId) const {
    auto it = userRatingIndex.find(userId);
    if (it == userRatingIndex.end()) return 0;
    return static_cast<int>(it->second.size());
}

// ── loadRatings ───────────────────────────────────────────────────────────────
// 변경 사항:
//   - findMovieById: O(M) → O(1) (movieIndex 사용)
//   - 평점 추가와 동시에 userRatingIndex 구축
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

        int    userId  = std::stoi(userIdStr);
        int    movieId = std::stoi(movieIdStr);
        double score   = std::stod(scoreStr);

        Movie*      movie = findMovieById(movieId); // O(1)
        const User* user  = userManager.findUserById(userId);

        if (movie && user) {
            movie->getRatingManager().addRating(Rating(*user, score));
            userRatingIndex[userId][movieId] = score; // 역색인 동시 구축
        }
    }
}

// ── saveRatings ───────────────────────────────────────────────────────────────
// userRatingIndex를 순회하면 Movie 전체를 돌지 않아도 되지만,
// score는 RatingManager에 이미 있으므로 기존 방식 유지 (정확성 우선)
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

// ── 아래는 변경 없음 ──────────────────────────────────────────────────────────

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
    std::sort(sortedMovies.begin(), sortedMovies.end(),
              [](const Movie& a, const Movie& b) {
                  return a.getAverageRating() > b.getAverageRating();
              });

    std::cout << "평점 순으로 정렬된 영화 목록:" << std::endl;
    for (const auto& movie : sortedMovies) {
        std::cout << movie
                  << " - Average Rating: " << std::fixed << std::setprecision(1) << movie.getAverageRating()
                  << std::endl;
    }
}

bool MovieManager::alreadyExists(const std::string& title,
                                 const std::string& genre, int year) const {
    for (const auto& movie : movies) {
        if (movie.getTitle() == title &&
            movie.getGenre() == genre &&
            movie.getReleaseYear() == year)
            return true;
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
    std::getline(movieFile, line); // 헤더 스킵

    while (std::getline(movieFile, line)) {
        std::istringstream ss(line);
        std::string idStr, title, genre, yearStr;
        std::getline(ss, idStr,   '|');
        std::getline(ss, title,   '|');
        std::getline(ss, genre,   '|');
        std::getline(ss, yearStr, '|');

        int id   = std::stoi(idStr);
        int year = std::stoi(yearStr);
        addMovie(Movie(id, title, genre, year)); // addMovie가 movieIndex도 갱신
    }
}

void MovieManager::saveToFile() {
    std::ofstream movieFile("data/movies.csv");
    if (!movieFile.is_open()) {
        std::cout << "movie.csv를 열 수 없습니다." << std::endl;
        return;
    }
    movieFile << "id|title|genre|releaseYear\n";
    for (const auto& movie : movies) {
        movieFile << movie.getId()         << "|"
                  << movie.getTitle()      << "|"
                  << movie.getGenre()      << "|"
                  << movie.getReleaseYear()<< "\n";
    }
}
