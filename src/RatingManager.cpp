#include "Rating.hpp"
#include "RatingManager.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
 
 
RatingManager::RatingManager() : totalRating(0.0), ratingCount(0), averageRating(0.0) {}
 
void RatingManager::addRating(const Rating& rating) {
    ratings.push_back(rating);
    totalRating += rating.getScore();
    ratingCount++;
    averageRating = totalRating / ratingCount;
}
 
// ── updateRating ──────────────────────────────────────────────────────────────
// 해당 userId 의 평점을 newScore 로 교체하고 평균을 재계산한다.
// 평점이 존재하지 않으면 false 를 반환한다.
bool RatingManager::updateRating(int userId, double newScore) {
    for (auto& r : ratings) {
        if (r.getUser().getId() == userId) {
            totalRating -= r.getScore();
            r.setScore(newScore);
            totalRating += newScore;
            averageRating = totalRating / ratingCount;
            return true;
        }
    }
    return false;
}
 
// ── hasRating ─────────────────────────────────────────────────────────────────
bool RatingManager::hasRating(int userId) const {
    for (const auto& r : ratings) {
        if (r.getUser().getId() == userId) return true;
    }
    return false;
}
 
int RatingManager::getRatingCount() const { return ratingCount; }
int RatingManager::size() const
{
    return ratings.size();
}
double RatingManager::getAverageRating() const { return averageRating; }
 
void RatingManager::display() const {
    for (const auto& r : ratings) {
        std::cout << "User: " << r.getUser().getName()
                  << ", Score: " << std::fixed << std::setprecision(1) << r.getScore() << std::endl;
    }
}
 
const std::vector<Rating>& RatingManager::getRatings() const {
    return ratings;
}
 
void RatingManager::sortRatingsByUserId() {
    std::sort(ratings.begin(), ratings.end(), [](const Rating& a, const Rating& b) {
        return a.getUser().getId() < b.getUser().getId();
    });
}