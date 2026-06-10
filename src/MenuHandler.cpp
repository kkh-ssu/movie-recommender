#include "MenuHandler.hpp"
#include "Movie.hpp"
#include "User.hpp"
#include "Rating.hpp"
#include "RatingManager.hpp"
#include "Recommender.hpp"
#include "Constants.hpp"
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <iomanip>
 
namespace MenuHandler {
 
// ── 내부 헬퍼 ────────────────────────────────────────────────────────────────
 
// 정수를 한 줄 입력받는다. 변환 실패 시 defaultVal 을 반환한다.
// ok 포인터가 있으면 성공 여부를 기록한다.
static int readInt(const std::string& prompt, bool* ok = nullptr) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    try {
        size_t pos;
        int val = std::stoi(line, &pos);
        // 뒤에 숫자 외 문자가 붙어 있으면 실패로 처리
        while (pos < line.size() && line[pos] == ' ') ++pos;
        if (pos != line.size()) throw std::invalid_argument("trailing chars");
        if (ok) *ok = true;
        return val;
    } catch (...) {
        if (ok) *ok = false;
        return 0;
    }
}
 
// 실수를 한 줄 입력받는다.
static double readDouble(const std::string& prompt, bool* ok = nullptr) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    try {
        size_t pos;
        double val = std::stod(line, &pos);
        while (pos < line.size() && line[pos] == ' ') ++pos;
        if (pos != line.size()) throw std::invalid_argument("trailing chars");
        if (ok) *ok = true;
        return val;
    } catch (...) {
        if (ok) *ok = false;
        return 0.0;
    }
}
 
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
              << "8. 영화별 평점 보기\n"
              << "12. 평점 수정\n"
              << "13. 내가 평가한 영화 목록\n\n";
    std::cout << "[ 추천 ]\n"
              << "9. 영화 추천\n"
              << "10. 장르별 TOP" << TOP_GENRE_COUNT << " 추천\n"
              << "11. 연도/장르 필터 추천\n\n";
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
    std::getline(std::cin, title);
 
    if (title.find('|') != std::string::npos) {
        std::cout << "'|' 는 사용할 수 없습니다. 다른 문자로 입력해주세요\n";
        return;
    }
 
    std::string genre;
    std::cout << "추가 할 영화 장르를 입력해주세요 : ";
    std::getline(std::cin, genre);
 
    // ── 연도 입력: 1895 ~ 2026 범위가 될 때까지 재입력 ─────────────────────
    int year = 0;
    while (true) {
        bool ok = false;
        year = readInt("추가 할 영화 출시 연도를 입력해주세요 : ", &ok);
        if (!ok) {
            std::cout << "정수를 입력해주세요.\n";
            continue;
        }
        if (year < MIN_RELEASE_YEAR || year > MAX_RELEASE_YEAR) {
            std::cout << MIN_RELEASE_YEAR << " ~ " << MAX_RELEASE_YEAR
                      << " 사이의 정수를 입력해주세요.\n";
            continue;
        }
        break;
    }
 
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
    std::getline(std::cin, title);
 
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
    std::getline(std::cin, name);
    std::cout << "추가 할 사용자 이메일을 입력해주세요 : ";
    std::getline(std::cin, email);
 
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
    std::getline(std::cin, title);
 
    Movie* movie = mm.findMovieByTitle(title);
    if (!movie) { std::cout << "영화를 찾을 수 없습니다.\n"; return; }
 
    std::cout << movie->getTitle() << "의 평점을 입력합니다\n";
    um.display();
 
    std::string name;
    std::cout << "평점을 입력할 사용자 이름을 입력해주세요 : ";
    std::getline(std::cin, name);
 
    if (!um.validUser(name)) { std::cout << "사용자를 찾을 수 없습니다.\n"; return; }
 
    const User* user = um.findUserByName(name);
 
    // 이미 평점을 남긴 경우 수정 여부 확인
    if (movie->getRatingManager().hasRating(user->getId())) {
        std::cout << "이미 평점을 남기셨습니다. 수정하시겠습니까? (y/n) : ";
        std::string ans;
        std::getline(std::cin, ans);
        if (ans != "y" && ans != "Y") {
            std::cout << "평점 입력을 취소합니다.\n";
            return;
        }
        // 수정 흐름으로 위임
        double score;
        while (true) {
            bool ok = false;
            score = readDouble("새 평점을 입력해주세요 (" +
                               std::to_string((int)MIN_SCORE) + " ~ " +
                               std::to_string((int)MAX_SCORE) + ") : ", &ok);
            if (!ok || score < MIN_SCORE || score > MAX_SCORE) {
                std::cout << MIN_SCORE << " ~ " << MAX_SCORE << " 사이의 값을 입력해주세요.\n";
                continue;
            }
            break;
        }
        movie->getRatingManager().updateRating(user->getId(), score);
        mm.updateUserRatingIndex(user->getId(), movie->getId(), score);
        std::cout << "평점이 수정되었습니다.\n";
        return;
    }
 
    double score;
    while (true) {
        bool ok = false;
        score = readDouble("평점을 입력해주세요 (" +
                           std::to_string((int)MIN_SCORE) + " ~ " +
                           std::to_string((int)MAX_SCORE) + ") : ", &ok);
        if (!ok || score < MIN_SCORE || score > MAX_SCORE) {
            std::cout << MIN_SCORE << " ~ " << MAX_SCORE << " 사이의 값을 입력해주세요.\n";
            continue;
        }
        break;
    }
 
    movie->getRatingManager().addRating(Rating(*user, score));
    mm.updateUserRatingIndex(user->getId(), movie->getId(), score);
    std::cout << "평점이 입력되었습니다.\n";
}
 
// ── 8. 영화별 평점 보기 ───────────────────────────────────────────────────────
void handleShowRatings(MovieManager& mm) {
    std::string title;
    std::cout << "평점을 볼 영화 제목을 입력해주세요 : ";
    std::getline(std::cin, title);
 
    const Movie* movie = mm.findMovieByTitle(title);
    if (!movie) { std::cout << "영화를 찾을 수 없습니다.\n"; return; }
 
    std::cout << movie->getTitle() << "의 평점 목록\n";
    movie->getRatingManager().display();
}
 
// ── 9. 영화 추천 ──────────────────────────────────────────────────────────────
void handleRecommend(MovieManager& mm, UserManager& um) {
    std::string name;
    std::cout << "추천받을 사용자 이름을 입력해주세요 : ";
    std::getline(std::cin, name);
 
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
 
    std::vector<std::string> genres;
    for (const auto& m : movies) {
        if (std::find(genres.begin(), genres.end(), m.getGenre()) == genres.end())
            genres.push_back(m.getGenre());
    }
    std::cout << "=== 장르 목록 ===\n";
    for (int i = 0; i < static_cast<int>(genres.size()); ++i)
        std::cout << i + 1 << ". " << genres[i] << "\n";
 
    bool ok = false;
    int genreNum = readInt("확인하고 싶은 장르 번호를 입력하세요 > ", &ok);
    if (!ok || genreNum < 1 || genreNum > static_cast<int>(genres.size())) {
        std::cout << "잘못된 번호입니다.\n";
        return;
    }
    const std::string& selectedGenre = genres[genreNum - 1];
 
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
 
// ── 11. 연도/장르 필터 추천 ──────────────────────────────────────────────────
// 연도 범위(a, b)와 장르를 선택적으로 받아 해당 조건을 만족하는 영화 중
// 평균 평점 상위 DEFAULT_TOP_N 개를 추천한다.
//
// 입력 규칙:
//   - 연도 시작(a): Enter → 하한 없음 / 정수 입력 → [a, ∞) 하한
//   - 연도 끝  (b): Enter → 상한 없음 / 정수 입력 → (-∞, b] 상한
//   - 장르        : Enter → 필터 없음 / 문자열 → 해당 장르만
void handleFilterRecommend(MovieManager& mm, UserManager& um) {
    // ── 연도 시작 입력 ───────────────────────────────────────────────────────
    std::cout << "연도 시작(a)을 입력하세요 (상관없으면 Enter) : ";
    std::string lineA;
    std::getline(std::cin, lineA);
 
    bool hasA = false;
    int yearA = 0;
    if (!lineA.empty()) {
        try {
            size_t pos;
            yearA = std::stoi(lineA, &pos);
            hasA  = true;
        } catch (...) {
            std::cout << "연도 시작 값을 파싱할 수 없어 하한을 적용하지 않습니다.\n";
        }
    }
 
    // ── 연도 끝 입력 ─────────────────────────────────────────────────────────
    std::cout << "연도 끝(b)을 입력하세요 (상관없으면 Enter) : ";
    std::string lineB;
    std::getline(std::cin, lineB);
 
    bool hasB = false;
    int yearB = 0;
    if (!lineB.empty()) {
        try {
            size_t pos;
            yearB = std::stoi(lineB, &pos);
            hasB  = true;
        } catch (...) {
            std::cout << "연도 끝 값을 파싱할 수 없어 상한을 적용하지 않습니다.\n";
        }
    }
 
    // a > b 인 경우 자동 교환
    if (hasA && hasB && yearA > yearB) {
        std::cout << "a > b 이므로 자동으로 교환합니다. ("
                  << yearA << " ↔ " << yearB << ")\n";
        std::swap(yearA, yearB);
    }
 
    // ── 장르 입력 ─────────────────────────────────────────────────────────────
    std::cout << "장르를 입력하세요 (상관없으면 Enter) : ";
    std::string genreFilter;
    std::getline(std::cin, genreFilter);
    bool hasGenre = !genreFilter.empty();
 
    // ── 조건 출력 ─────────────────────────────────────────────────────────────
    std::cout << "\n[적용된 필터]\n";
    if (hasA || hasB) {
        std::cout << "  연도 : ";
        if (hasA)  std::cout << yearA; else std::cout << "-∞";
        std::cout << " ~ ";
        if (hasB)  std::cout << yearB; else std::cout << "+∞";
        std::cout << "\n";
    } else {
        std::cout << "  연도 : 제한 없음\n";
    }
    std::cout << "  장르 : " << (hasGenre ? genreFilter : "제한 없음") << "\n\n";
 
    // ── 필터링 ────────────────────────────────────────────────────────────────
    std::vector<const Movie*> candidates;
    for (const auto& m : mm.getMovies()) {
        if (hasA && m.getReleaseYear() < yearA) continue;
        if (hasB && m.getReleaseYear() > yearB) continue;
        if (hasGenre && m.getGenre() != genreFilter) continue;
        if (m.getAverageRating() <= MIN_SCORE)  continue; // 평점 없는 영화 제외
        candidates.push_back(&m);
    }
 
    if (candidates.empty()) {
        std::cout << "조건에 맞는 평점 있는 영화가 없습니다.\n";
        return;
    }
 
    // 평균 평점 내림차순 정렬 후 상위 DEFAULT_TOP_N 개 출력
    std::sort(candidates.begin(), candidates.end(),
              [](const Movie* a, const Movie* b) {
                  return a->getAverageRating() > b->getAverageRating();
              });
 
    int showCount = std::min(DEFAULT_TOP_N, static_cast<int>(candidates.size()));
    std::cout << "필터 조건 평점 TOP" << showCount << "\n";
    for (int i = 0; i < showCount; ++i) {
        std::cout << i + 1 << ". " << candidates[i]->getTitle()
                  << "  (" << candidates[i]->getReleaseYear()
                  << " / " << candidates[i]->getGenre()
                  << " / 평균 평점: "
                  << std::fixed << std::setprecision(RATING_PRECISION)
                  << candidates[i]->getAverageRating() << ")\n";
    }
}
 
// ── 12. 평점 수정 ─────────────────────────────────────────────────────────────
// 영화 제목과 사용자 이름을 받아 기존 평점을 새 값으로 교체한다.
// 평점이 없으면 새로 추가할지 확인한다.
void handleUpdateRating(MovieManager& mm, UserManager& um) {
    std::string title;
    std::cout << "평점을 수정할 영화 제목을 입력해주세요 : ";
    std::getline(std::cin, title);
 
    Movie* movie = mm.findMovieByTitle(title);
    if (!movie) { std::cout << "영화를 찾을 수 없습니다.\n"; return; }
 
    um.display();
    std::string name;
    std::cout << "수정할 사용자 이름을 입력해주세요 : ";
    std::getline(std::cin, name);
 
    if (!um.validUser(name)) { std::cout << "사용자를 찾을 수 없습니다.\n"; return; }
 
    const User* user = um.findUserByName(name);
 
    // 평점이 없는 경우
    if (!movie->getRatingManager().hasRating(user->getId())) {
        std::cout << "해당 사용자의 평점이 없습니다. 새로 입력하시겠습니까? (y/n) : ";
        std::string ans;
        std::getline(std::cin, ans);
        if (ans != "y" && ans != "Y") {
            std::cout << "취소합니다.\n";
            return;
        }
        double score;
        while (true) {
            bool ok = false;
            score = readDouble("평점을 입력해주세요 (" +
                               std::to_string((int)MIN_SCORE) + " ~ " +
                               std::to_string((int)MAX_SCORE) + ") : ", &ok);
            if (!ok || score < MIN_SCORE || score > MAX_SCORE) {
                std::cout << MIN_SCORE << " ~ " << MAX_SCORE << " 사이의 값을 입력해주세요.\n";
                continue;
            }
            break;
        }
        movie->getRatingManager().addRating(Rating(*user, score));
        mm.updateUserRatingIndex(user->getId(), movie->getId(), score);
        std::cout << "평점이 입력되었습니다.\n";
        return;
    }
 
    double score;
    while (true) {
        bool ok = false;
        score = readDouble("새 평점을 입력해주세요 (" +
                           std::to_string((int)MIN_SCORE) + " ~ " +
                           std::to_string((int)MAX_SCORE) + ") : ", &ok);
        if (!ok || score < MIN_SCORE || score > MAX_SCORE) {
            std::cout << MIN_SCORE << " ~ " << MAX_SCORE << " 사이의 값을 입력해주세요.\n";
            continue;
        }
        break;
    }
 
    movie->getRatingManager().updateRating(user->getId(), score);
    mm.updateUserRatingIndex(user->getId(), movie->getId(), score);
    std::cout << "평점이 수정되었습니다.\n";
}
 
// ── 13. 내가 평가한 영화 목록 ────────────────────────────────────────────────
// 사용자 이름을 받아 해당 사용자가 평점을 남긴 영화를 평점 내림차순으로 출력한다.
void handleShowUserRatings(MovieManager& mm, UserManager& um) {
    um.display();
    std::string name;
    std::cout << "조회할 사용자 이름을 입력해주세요 : ";
    std::getline(std::cin, name);
 
    const User* user = um.findUserByName(name);
    if (!user) { std::cout << "사용자를 찾을 수 없습니다.\n"; return; }
 
    const auto& ratingMap = mm.getUserRatings(user->getId());
    if (ratingMap.empty()) {
        std::cout << user->getName() << "님이 평가한 영화가 없습니다.\n";
        return;
    }
 
    // { score, movieId } 내림차순으로 정렬해 출력
    std::vector<std::pair<double, int>> sorted(ratingMap.size());
    int idx = 0;
    for (const auto& [movieId, score] : ratingMap)
        sorted[idx++] = {score, movieId};
 
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });
 
    std::cout << "\n" << user->getName() << "님이 평가한 영화 목록 (평점 높은 순)\n";
    std::cout << std::string(40, '-') << "\n";
    for (const auto& [score, movieId] : sorted) {
        const Movie* movie = mm.findMovieById(movieId);
        if (!movie) continue;
        std::cout << "  " << movie->getTitle()
                  << "  (" << movie->getReleaseYear()
                  << " / " << movie->getGenre() << ")"
                  << "  → 내 평점: "
                  << std::fixed << std::setprecision(RATING_PRECISION)
                  << score << "\n";
    }
    std::cout << std::string(40, '-') << "\n";
    std::cout << "총 " << ratingMap.size() << "편 평가\n";
}
 
} // namespace MenuHandler