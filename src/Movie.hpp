#pragma once
#include <string>
#include "RatingManager.hpp"

class Movie {
private:
    int           id;
    std::string   title;
    std::string   genre;
    int           releaseYear;
    RatingManager ratingManager;

public:
    Movie();
    Movie(int id, const std::string& title,
          const std::string& genre, int year);

    // 큰 객체(string)는 const& 로 반환해 불필요한 복사 방지
    int                  getId()          const;
    const std::string&   getTitle()       const;
    const std::string&   getGenre()       const;
    int                  getReleaseYear() const;
    double               getAverageRating() const;

    // 평점 추가용 non-const / 조회용 const — 두 오버로드 모두 필요
    RatingManager&       getRatingManager();
    const RatingManager& getRatingManager() const;

    bool operator==(const Movie& o) const;
    bool operator!=(const Movie& o) const;
    bool operator< (const Movie& o) const;
    bool operator> (const Movie& o) const;
    bool operator<=(const Movie& o) const;
    bool operator>=(const Movie& o) const;

    friend std::ostream& operator<<(std::ostream& os, const Movie& m);
};