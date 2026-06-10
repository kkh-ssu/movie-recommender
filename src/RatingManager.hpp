#pragma once
#include <vector>
#include "Rating.hpp"
 
class RatingManager {
private:
    std::vector<Rating> ratings;
    double totalRating;
    int ratingCount;
    double averageRating;
public:
    RatingManager();
    void addRating(const Rating& rating);
    // userId 기준으로 기존 평점을 newScore로 교체. 해당 유저 평점이 없으면 false 반환.
    bool updateRating(int userId, double newScore);
    // userId 가 이미 평점을 남겼는지 확인
    bool hasRating(int userId) const;
 
    int getRatingCount() const;
    const std::vector<Rating>& getRatings() const;
    void sortRatingsByUserId();
 
    int size() const;
    double getAverageRating() const;
    void display() const;
};