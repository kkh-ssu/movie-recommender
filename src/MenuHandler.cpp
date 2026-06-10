#include "MenuHandler.hpp"
#include "Movie.hpp"
#include "User.hpp"
#include "Rating.hpp"
#include "RatingManager.hpp"
#include "Recommender.hpp"
#include "Constants.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

namespace MenuHandler {

// ── 메뉴 출력 ─────────────────────────────────────────────────────────────────
void printMenu() {
    std::cout << "=== Movie Recommender ===\n\n";
    std::cout << "[ 영화 ]\n"
              << "1. 영화 추가\n"
              << "2. 제목으로 검색\n"
              << "3. 전체 목록 출력\n"
              << "4. 평점순 출력\n\n";
    std::cout << "[ 사용자 ]\n"
              << "5. 사용자 추가\n"
              << "6. 사용자 목록 출력\n\n";
    std::cout << "[ 평점 ]\n"
              << "7. 평점 입력\n"
              << "8. 영화별 평점 보기\n\n";
    std::cout << "[ 추천 ]\n"
              << "9. 영화 추천\n"
              << "10. 장르별 TOP" << TOP_GENRE_COUNT << " 추천\n\n";
    std::cout << "0. 종료\n\n";
}

// ── 경고 출력 ─────────────────────────────────────────────────────────────────
void printWarnings(const MovieManager& mm, const UserManager& um) {
    if (mm.getMovieCount() == 0)
        std::cout << "경고: 등록된 영화가 없습니다. 영화를 추가해주세요.\n\n";
    if (um.getUserCount() == 0)
        std::cout << "경고: 등록된 사용자가 없습니다. 사용자를 추가해주세요.\n\n";
}

// ── 1. 영화 추가 ──────────────────────────────────────────────────────────────
void handleAddMovie(MovieManager& mm, UserManager&) {
    std::string title;
    std::cout << "추가 할 영화 제목을 입력해주세요 : ";
    std::cin >> title;

    // '|'는 CSV 구분자로 사용 중이므로 입력 불가
    if (title.find('|') != std::string::npos) {
        std::cout << "'|' 는 사용할 수 없습니다. 다른 문자로 입력해주세요\n";
        return;
    }

    std::string genre;
    std::cout << "추가 할 영화 장르를 입력해주세요 : ";
    std::cin >> genre;

    int year;
    std::cout << "추가 할 영화 출시 연도를 입력해주세요 : ";
    std::cin >> year;

    if (mm.alreadyExists(title, genre, year)) {
        std::cout << "이미 존재하는 영화입니다.\n";
        return;
    }

    mm.addMovie(Movie(mm.getMovieCount() + 1, title, genre, year));
    std::cout << "영화가 추가되었습니다.\n";
}

// ── 2. 제목으로 검색 ──────────────────────────────────────────────────────────
void handleSearchMovie(MovieManager& mm) {
    std::string title;
    std::cout << "검색 할 영화 제목을 입력해주세요 : ";
    std::cin >> title;

    const Movie* found = mm.findMovieByTitle(title);
    if (found)
        std::cout << *found << std::endl;
    else
        std::cout << "영화를 찾을 수 없습니다.\n";
}

// ── 3. 전체 목록 출력 ─────────────────────────────────────────────────────────
void handleListMovies(MovieManager& mm) {
    if (mm.getMovieCount() == 0) {
        std::cout << "등록된 영화가 없습니다.\n";
        return;
    }
    mm.display();
}

// ── 4. 평점순 출력 ────────────────────────────────────────────────────────────
void handleListByRating(MovieManager& mm) {
    if (mm.getMovieCount() == 0) {
        std::cout << "등록된 영화가 없습니다.\n";
        return;
    }
    mm.displaySortedByRating();
}

// ── 5. 사용자 추가 ────────────────────────────────────────────────────────────
void handleAddUser(UserManager& um) {
    std::string name, email;
    std::cout << "추가 할 사용자 이름을 입력해주세요 : ";
    std::cin >> name;
    std::cout << "추가 할 사용자 이메일을 입력해주세요 : ";
    std::cin >> email;

    if (um.validUser(name, email)) {
        std::cout << "이미 존재하는 사용자입니다.\n";
        return;
    }

    um.addUser(User(um.getUserCount() + 1, name, email));
    std::cout << "사용자가 추가되었습니다.\n";
}

// ── 6. 사용자 목록 출력 ───────────────────────────────────────────────────────
void handleListUsers(UserManager& um) {
    if (um.getUserCount() == 0) {
        std::cout << "등록된 사용자가 없습니다.\n";
        return;
    }
    um.display();
}

// ── 7. 평점 입력 ──────────────────────────────────────────────────────────────
void handleAddRating(MovieManager& mm, UserManager& um) {
    std::string title;
    std::cout << "평점을 입력할 영화 제목을 입력해주세요 : ";
    std::cin >> title;

    Movie* movie = mm.findMovieByTitle(title);
    if (!movie) { std::cout << "영화를 찾을 수 없습니다.\n"; return; }

    std::cout << movie->getTitle() << "의 평점을 입력합니다\n";
    um.display();

    std::string name;
    std::cout << "평점을 입력할 사용자 이름을 입력해주세요 : ";
    std::cin >> name;

    if (!um.validUser(name)) { std::cout << "사용자를 찾을 수 없습니다.\n"; return; }

    double score;
    std::cout << "평점을 입력해주세요 (" << MIN_SCORE << " ~ " << MAX_SCORE << ") : ";
    std::cin >> score;

    if (score < MIN_SCORE || score > MAX_SCORE) {
        std::cout << "잘못된 평점입니다.\n";
        return;
    }

    const User* user = um.findUserByName(name);
    movie->getRatingManager().addRating(Rating(*user, score));
    std::cout << "평점이 입력되었습니다.\n";
}

// ── 8. 영화별 평점 보기 ───────────────────────────────────────────────────────
void handleShowRatings(MovieManager& mm) {
    std::string title;
    std::cout << "평점을 볼 영화 제목을 입력해주세요 : ";
    std::cin >> title;

    const Movie* movie = mm.findMovieByTitle(title);
    if (!movie) { std::cout << "영화를 찾을 수 없습니다.\n"; return; }

    std::cout << movie->getTitle() << "의 평점 목록\n";
    movie->getRatingManager().display();
}

// ── 9. 영화 추천 ──────────────────────────────────────────────────────────────
void handleRecommend(MovieManager& mm, UserManager& um) {
    std::string name;
    std::cout << "추천받을 사용자 이름을 입력해주세요 : ";
    std::cin >> name;

    const User* user = um.findUserByName(name);
    if (!user) { std::cout << "사용자를 찾을 수 없습니다.\n"; return; }

    auto recommendations = Recommender::recommend(user->getId(), um, mm);
    if (recommendations.empty()) {
        std::cout << "추천할 영화가 없습니다.\n";
        return;
    }

    std::cout << user->getName() << "님을 위한 추천 영화:\n";
    for (const auto* movie : recommendations)
        std::cout << *movie << std::endl;
}

// ── 10. 장르별 TOP N 추천 ────────────────────────────────────────────────────
void handleGenreTopN(MovieManager& mm) {
    const auto& movies = mm.getMovies();

    // 중복 없이 장르 수집 후 번호 출력
    std::vector<std::string> genres;
    for (const auto& m : movies) {
        if (std::find(genres.begin(), genres.end(), m.getGenre()) == genres.end())
            genres.push_back(m.getGenre());
    }
    std::cout << "=== 장르 목록 ===\n";
    for (int i = 0; i < static_cast<int>(genres.size()); ++i)
        std::cout << i + 1 << ". " << genres[i] << "\n";

    int genreNum;
    std::cout << "확인하고 싶은 장르 번호를 입력하세요 > ";
    std::cin >> genreNum;
    if (genreNum < 1 || genreNum > static_cast<int>(genres.size())) {
        std::cout << "잘못된 번호입니다.\n";
        return;
    }
    const std::string& selectedGenre = genres[genreNum - 1];

    // 선택 장르 중 평점 있는 영화를 평균 평점 내림차순 정렬
    std::vector<const Movie*> candidates;
    for (const auto& m : movies) {
        if (m.getGenre() == selectedGenre && m.getAverageRating() > MIN_SCORE)
            candidates.push_back(&m);
    }
    if (candidates.empty()) {
        std::cout << "[" << selectedGenre << "] 장르에 평점이 있는 영화가 없습니다.\n";
        return;
    }
    std::sort(candidates.begin(), candidates.end(),
              [](const Movie* a, const Movie* b) {
                  return a->getAverageRating() > b->getAverageRating();
              });

    int showCount = std::min(TOP_GENRE_COUNT, static_cast<int>(candidates.size()));
    std::cout << "\n[" << selectedGenre << "] 장르 평점 TOP" << showCount << "\n";
    for (int i = 0; i < showCount; ++i) {
        std::cout << i + 1 << ". " << candidates[i]->getTitle()
                  << "  (평균 평점: "
                  << std::fixed << std::setprecision(RATING_PRECISION)
                  << candidates[i]->getAverageRating() << ")\n";
    }
}

} // namespace MenuHandler