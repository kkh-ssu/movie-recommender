#include "Movie.hpp"
#include "Constants.hpp"
#include <iostream>
#include <iomanip>
 
Movie::Movie()
    : id(0), releaseYear(0) {}
 
Movie::Movie(int id, const std::string& title,
             const std::string& genre, int year)
    : id(id), title(title), genre(genre), releaseYear(year) {}
 
int                  Movie::getId()          const { return id; }
const std::string&   Movie::getTitle()       const { return title; }
const std::string&   Movie::getGenre()       const { return genre; }
int                  Movie::getReleaseYear() const { return releaseYear; }
double               Movie::getAverageRating() const { return ratingManager.getAverageRating(); }
RatingManager&       Movie::getRatingManager()       { return ratingManager; }
const RatingManager& Movie::getRatingManager() const { return ratingManager; }
 
bool Movie::operator==(const Movie& o) const { return id == o.id; }
bool Movie::operator!=(const Movie& o) const { return !(*this == o); }
bool Movie::operator< (const Movie& o) const { return getAverageRating() <  o.getAverageRating(); }
bool Movie::operator> (const Movie& o) const { return getAverageRating() >  o.getAverageRating(); }
bool Movie::operator<=(const Movie& o) const { return getAverageRating() <= o.getAverageRating(); }
bool Movie::operator>=(const Movie& o) const { return getAverageRating() >= o.getAverageRating(); }
 
std::ostream& operator<<(std::ostream& os, const Movie& m) {
    os << "id: "       << m.id
       << " 제목: "    << m.title
       << " 장르: "    << m.genre
       << " 출시 연도: " << m.releaseYear << "\n"
       << "평균 평점: " << std::fixed
                       << std::setprecision(RATING_PRECISION)
                       << m.ratingManager.getAverageRating()
       << " 평가 수: " << m.ratingManager.getRatingCount();
    return os;
}