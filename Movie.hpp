#pragma once
#include <string>
#include "RatingManager.hpp"

class Movie {
private:
    int         id;
    std::string title;
    std::string genre;
    int         releaseYear;

    RatingManager ratingManager; // 추가
public:
    Movie();                    // 기본 생성자 추가
    Movie(int id, const std::string& title,
          const std::string& genre, int year);

    int         getId()              const;
    std::string getTitle()           const;
    std::string getGenre()           const;
    int         getReleaseYear()     const;  // getYear → getReleaseYear
    int         getRatingCount()     const;  // 추가

    void addRating(double r);               // 추가

    bool operator==(const Movie& o) const;
    bool operator!=(const Movie& o) const;
    bool operator< (const Movie& o) const;
    bool operator> (const Movie& o) const;
    bool operator<=(const Movie& o) const;
    bool operator>=(const Movie& o) const;

    friend std::ostream& operator<<(std::ostream& os, const Movie& m);

};
