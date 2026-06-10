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
#include <functional>

namespace MenuHandler {

// ── 내부 헬퍼 ────────────────────────────────────────────────────────────────

static int readInt(const std::string& prompt, bool* ok = nullptr) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    try {
        size_t pos;
        int val = std::stoi(line, &pos);
        while (pos < line.size() && line[pos] == ' ') ++pos;
        if (pos != line.size()) throw std::invalid_argument("trailing chars");
        if (ok) *ok = true;
        return val;
    } catch (...) {
        if (ok) *ok = false;
        return 0;
    }
}

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

// ── 페이지네이션 헬퍼 ─────────────────────────────────────────────────────────
// items      : 출력할 항목 벡터
// header     : 첫 줄에 출력할 제목 (예: "영화 목록")
// printItem  : 각 항목을 출력하는 함수 (1-based 번호, 항목 참조)
// PAGE_SIZE  : 한 페이지에 보여줄 항목 수 (Constants.hpp의 PAGE_SIZE 사용)
template <typename T>
static void paginate(const std::vector<T>& items,
                     const std::string& header,
                     const std::function<void(int, const T&)>& printItem) {
    if (items.empty()) return;

    int total = static_cast<int>(items.size());
    int totalPages = (total + PAGE_SIZE - 1) / PAGE_SIZE;
    int page = 0; // 0-based

    while (true) {
        int start = page * PAGE_SIZE;
        int end   = std::min(start + PAGE_SIZE, total);
        //헤더
        std::cout << "\n" << header
                  << "  [" << (page + 1) << "/" << totalPages << "페이지"
                  << ", 총 " << total << "개]\n";
        std::cout << std::string(50, '-') << "\n";
        //헤더============================================================
        //목록
        for (int i = start; i < end; ++i)
            printItem(i + 1, items[i]);

        std::cout << std::string(50, '-') << "\n";
        //목록============================================================

        // 네비게이션 안내
        bool hasPrev = (page > 0);
        bool hasNext = (page < totalPages - 1);

        if (!hasNext) {
            // 마지막 페이지 — 이전 페이지만 가능하면 안내
            if (hasPrev)
                std::cout << "[p: 이전 / 그 외: 종료] > ";
            else
                std::cout << "목록 끝. Enter를 누르면 돌아갑니다. > ";
        } else if (!hasPrev) {
            std::cout << "[n: 다음 / 그 외: 종료] > ";
        } else {
            std::cout << "[n: 다음 / p: 이전 / 그 외: 종료] > ";
        }
        //네비게이션 안내 ============================================================
        std::string cmd;
        std::getline(std::cin, cmd);

        if ((cmd == "n" || cmd == "N") && hasNext)
            ++page;
        else if ((cmd == "p" || cmd == "P") && hasPrev)
            --page;
        else
            break;
    }
}


// ── 사용자 목록 간이 출력 (평점 입력·수정 시 선택용, 페이지네이션 적용) ───────
static void displayUsersPaged(const UserManager& um) {
    if (um.getUserCount() == 0) return;
    const auto& users = um.getUsers();
    paginate<User>(users, "사용자 목록",
        [](int no, const User& u) {
            std::cout << no << ". " << u.getName()
                      << "  (ID: " << u.getId() << ")\n";
        });
}

static void displayMoviesPaged(const MovieManager& mm) {
    if (mm.getMovieCount() == 0) return;
    const auto& movies = mm.getMovies();
    paginate<Movie>(movies, "영화 목록",
        [](int no, const Movie& m) {
            std::cout << no << ". " << m.getTitle()
                      << "  (ID: " << m.getId() << ")\n";
        });
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
              << "9. 평점 수정\n"
              << "10. 내가 평가한 영화 목록\n\n";
    std::cout << "[ 추천 ]\n"
              << "11. 영화 추천\n"
              << "12. 장르별 TOP" << TOP_GENRE_COUNT << " 추천\n"
              << "13. 연도/장르 필터 추천\n\n";
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

// ── 3. 전체 목록 출력 (페이지네이션) ─────────────────────────────────────────
void handleListMovies(MovieManager& mm) {
    if (mm.getMovieCount() == 0) {
        std::cout << "등록된 영화가 없습니다.\n";
        return;
    }
    const auto& movies = mm.getMovies();
    paginate<Movie>(movies, "영화 목록",
        [](int no, const Movie& m) {
            std::cout << no << ". " << m << "\n";
        });
}

// ── 4. 평점순 출력 (페이지네이션) ────────────────────────────────────────────
void handleListByRating(MovieManager& mm) {
    if (mm.getMovieCount() == 0) {
        std::cout << "등록된 영화가 없습니다.\n";
        return;
    }
    // 정렬된 복사본을 만들어 paginate에 넘긴다
    std::vector<Movie> sorted = mm.getMovies();
    std::sort(sorted.begin(), sorted.end(),
              [](const Movie& a, const Movie& b) { return a > b; });

    paginate<Movie>(sorted, "영화 목록 (평점순)",
        [](int no, const Movie& m) {
            std::cout << no << ". " << m
                      << "  - 평균 평점: "
                      << std::fixed << std::setprecision(RATING_PRECISION)
                      << m.getAverageRating() << "\n";
        });
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

// ── 6. 사용자 목록 출력 (페이지네이션) ───────────────────────────────────────
void handleListUsers(UserManager& um) {
    if (um.getUserCount() == 0) {
        std::cout << "등록된 사용자가 없습니다.\n";
        return;
    }
    const auto& users = um.getUsers();
    paginate<User>(users, "사용자 목록",
        [](int no, const User& u) {
            std::cout << no << ". ID: " << u.getId()
                      << "  이름: " << u.getName()
                      << "  이메일: " << u.getEmail() << "\n";
        });
}

// ── 7. 평점 입력 ──────────────────────────────────────────────────────────────
void handleAddRating(MovieManager& mm, UserManager& um) {

    displayMoviesPaged(mm);

    std::string title;
    std::cout << "평점을 입력할 영화 제목을 입력해주세요 : ";
    std::getline(std::cin, title);

    Movie* movie = mm.findMovieByTitle(title);
    if (!movie) { std::cout << "영화를 찾을 수 없습니다.\n"; return; }

    std::cout << movie->getTitle() << "의 평점을 입력합니다\n";
    displayUsersPaged(um);

    std::string name;
    std::cout << "평점을 입력할 사용자 이름을 입력해주세요 : ";
    std::getline(std::cin, name);

    if (!um.validUser(name)) { std::cout << "사용자를 찾을 수 없습니다.\n"; return; }

    const User* user = um.findUserByName(name);

    if (movie->getRatingManager().hasRating(user->getId())) {
        std::cout << "이미 평점을 남기셨습니다. 수정하시겠습니까? (y/n) : ";
        std::string ans;
        std::getline(std::cin, ans);
        if (ans != "y" && ans != "Y") {
            std::cout << "평점 입력을 취소합니다.\n";
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

// ── 8. 영화별 평점 보기 (페이지네이션) ───────────────────────────────────────
void handleShowRatings(MovieManager& mm) {
    std::string title;
    std::cout << "평점을 볼 영화 제목을 입력해주세요 : ";
    std::getline(std::cin, title);

    const Movie* movie = mm.findMovieByTitle(title);
    if (!movie) { std::cout << "영화를 찾을 수 없습니다.\n"; return; }

    const auto& ratings = movie->getRatingManager().getRatings();
    if (ratings.empty()) {
        std::cout << movie->getTitle() << "의 평점이 없습니다.\n";
        return;
    }

    paginate<Rating>(ratings, movie->getTitle() + " 평점 목록",
        [](int no, const Rating& r) {
            std::cout << no << ". "
                      << r.getUser().getName()
                      << "  →  "
                      << std::fixed << std::setprecision(1)
                      << r.getScore() << "\n";
        });
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
void handleFilterRecommend(MovieManager& mm, UserManager& um) {
    // ── 연도 시작 입력: 빈 값(Enter) → 하한 없음 / 정수 외 → 재입력 ──────────
    bool hasA = false;
    int yearA = 0;
    while (true) {
        std::cout << "연도 시작(a)을 입력하세요 (상관없으면 Enter) : ";
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) { hasA = false; break; }
        try {
            size_t pos;
            yearA = std::stoi(line, &pos);
            while (pos < line.size() && line[pos] == ' ') ++pos;
            if (pos != line.size()) throw std::invalid_argument("");
            hasA = true;
            break;
        } catch (...) {
            std::cout << "정수를 입력하거나 Enter로 건너뛰세요.\n";
        }
    }

    // ── 연도 끝 입력: 동일 규칙 ──────────────────────────────────────────────
    bool hasB = false;
    int yearB = 0;
    while (true) {
        std::cout << "연도 끝(b)을 입력하세요 (상관없으면 Enter) : ";
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) { hasB = false; break; }
        try {
            size_t pos;
            yearB = std::stoi(line, &pos);
            while (pos < line.size() && line[pos] == ' ') ++pos;
            if (pos != line.size()) throw std::invalid_argument("");
            hasB = true;
            break;
        } catch (...) {
            std::cout << "정수를 입력하거나 Enter로 건너뛰세요.\n";
        }
    }

    if (hasA && hasB && yearA > yearB) {
        std::cout << "a > b 이므로 자동으로 교환합니다. ("
                  << yearA << " ↔ " << yearB << ")\n";
        std::swap(yearA, yearB);
    }

    // ── 장르 선택: 목록 출력 후 번호 입력, 0이면 필터 없음, 범위 밖이면 재입력 ─
    std::vector<std::string> genres;
    for (const auto& m : mm.getMovies()) {
        if (std::find(genres.begin(), genres.end(), m.getGenre()) == genres.end())
            genres.push_back(m.getGenre());
    }

    std::string genreFilter;
    bool hasGenre = false;

    std::cout << "\n=== 장르 목록 ===\n";
    std::cout << "0. 상관없음\n";
    for (int i = 0; i < static_cast<int>(genres.size()); ++i)
        std::cout << i + 1 << ". " << genres[i] << "\n";

    while (true) {
        bool ok = false;
        int pick = readInt("장르 번호를 입력하세요 > ", &ok);
        if (!ok || pick < 0 || pick > static_cast<int>(genres.size())) {
            std::cout << "0 ~ " << genres.size() << " 사이의 번호를 입력하세요.\n";
            continue;
        }
        if (pick == 0) { hasGenre = false; }
        else           { hasGenre = true; genreFilter = genres[pick - 1]; }
        break;
    }

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

    std::vector<const Movie*> candidates;
    for (const auto& m : mm.getMovies()) {
        if (hasA && m.getReleaseYear() < yearA) continue;
        if (hasB && m.getReleaseYear() > yearB) continue;
        if (hasGenre && m.getGenre() != genreFilter) continue;
        if (m.getAverageRating() <= MIN_SCORE)  continue;
        candidates.push_back(&m);
    }

    if (candidates.empty()) {
        std::cout << "조건에 맞는 평점 있는 영화가 없습니다.\n";
        return;
    }

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
void handleUpdateRating(MovieManager& mm, UserManager& um) {
    std::string title;
    std::cout << "평점을 수정할 영화 제목을 입력해주세요 : ";
    std::getline(std::cin, title);

    Movie* movie = mm.findMovieByTitle(title);
    if (!movie) { std::cout << "영화를 찾을 수 없습니다.\n"; return; }

    displayUsersPaged(um);
    std::string name;
    std::cout << "수정할 사용자 이름을 입력해주세요 : ";
    std::getline(std::cin, name);

    if (!um.validUser(name)) { std::cout << "사용자를 찾을 수 없습니다.\n"; return; }

    const User* user = um.findUserByName(name);

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

// ── 13. 내가 평가한 영화 목록 (페이지네이션) ─────────────────────────────────
void handleShowUserRatings(MovieManager& mm, UserManager& um) {
    displayUsersPaged(um);
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

    // { score, movieId } 내림차순 정렬
    using Entry = std::pair<double, int>;
    std::vector<Entry> sorted;
    sorted.reserve(ratingMap.size());
    for (const auto& [movieId, score] : ratingMap)
        sorted.push_back({score, movieId});
    std::sort(sorted.begin(), sorted.end(),
              [](const Entry& a, const Entry& b) { return a.first > b.first; });

    paginate<Entry>(sorted,
        user->getName() + "님이 평가한 영화 목록 (평점 높은 순)",
        [&mm](int no, const Entry& e) {
            const Movie* movie = mm.findMovieById(e.second);
            if (!movie) return;
            std::cout << no << ". " << movie->getTitle()
                      << "  (" << movie->getReleaseYear()
                      << " / " << movie->getGenre() << ")"
                      << "  → 내 평점: "
                      << std::fixed << std::setprecision(RATING_PRECISION)
                      << e.first << "\n";
        });
}

} // namespace MenuHandler