#include "Movie.hpp"
#include <iostream>
#include "RatingManager.hpp"

// 기본 생성자
Movie::Movie()
    : id(0), releaseYear(0),
      ratingManager() {}

// 4인자 생성자
Movie::Movie(int id, const std::string& title,
             const std::string& genre, int year)
    : id(id), title(title), genre(genre),
      releaseYear(year),
      ratingManager(RatingManager()) {}

int         Movie::getId()           const { return id; }
std::string Movie::getTitle()        const { return title; }
std::string Movie::getGenre()        const { return genre; }
int         Movie::getReleaseYear()  const { return releaseYear; }
double Movie::getAverageRating() const { return ratingManager.getAverageRating(); }
RatingManager& Movie::getRatingManager() { return ratingManager; }
const RatingManager& Movie::getRatingManager() const { return ratingManager; } 

bool Movie::operator==(const Movie& o) const {
    return id == o.id;
}

bool Movie::operator!=(const Movie& o) const {
    return !(*this == o);
}

bool Movie::operator<(const Movie& o) const {
    return this->getAverageRating() < o.getAverageRating();
}

bool Movie::operator>(const Movie& o) const {
    return this->getAverageRating() > o.getAverageRating();
}

bool Movie::operator<=(const Movie& o) const {
    return !(this->getAverageRating() > o.getAverageRating());
}

bool Movie::operator>=(const Movie& o) const {
    return !(this->getAverageRating() < o.getAverageRating());
}

std::ostream& operator<<(std::ostream& os, const Movie& m) {
    os << "ID: " << m.id << "\n"
       << "제목: " << m.title << "\n"
       << "장르: " << m.genre << "\n"
       << "출시 연도: " << m.releaseYear << "\n"           
       << "평균 평점: " << m.ratingManager.getAverageRating() << "\n"
       << "평가 수: " << m.ratingManager.getRatingCount();
    return os;
}