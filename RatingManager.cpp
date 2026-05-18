#include "Rating.hpp"
#include "RatingManager.hpp"
#include <iostream>
#include <sstream>
#include <fstream>


RatingManager::RatingManager() : totalRating(0.0), ratingCount(0), averageRating(0.0) {}

void RatingManager::addRating(const Rating& rating) {
    ratings.push_back(rating);
    totalRating += rating.getScore();
    ratingCount++;
    averageRating = totalRating / ratingCount;
}

int RatingManager::getRatingCount() const { return ratingCount; }
double RatingManager::getAverageRating() const { return averageRating; }

void RatingManager::display() const {
    for (const auto& r : ratings) {
        std::cout << "User: " << r.getUser().getName()
                  << ", Score: " << r.getScore() << std::endl;
    }
}

void RatingManager::loadFromfile(UserManager& userManager) {
    std::ifstream ratingFile("data/ratings.csv");
    if (!ratingFile.is_open()) {
        std::cout << "ratings.csv를 열 수 없습니다." << std::endl;
        return;
    }
    std::string line;
    std::getline(ratingFile, line);

    while (std::getline(ratingFile, line)) {
        std::istringstream ss(line);
        std::string userIdStr, scoreStr;
        std::getline(ss, userIdStr, ',');
        std::getline(ss, scoreStr, ',');

        int userId = std::stoi(userIdStr);
        double score = std::stod(scoreStr);

        const User* user = userManager.findUserById(userId);
        if (user) {
            addRating(Rating(*user, score));
        }
    }
}

void RatingManager::saveToFile(UserManager& userManager) {
    std::ofstream ratingFile("data/ratings.csv");
    if (!ratingFile.is_open()) {
        std::cout << "ratings.csv를 열 수 없습니다." << std::endl;
        return;
    }
    ratingFile << "userId,score\n";
    for (const auto& r : ratings) {
        ratingFile << r.getUser().getId() << "," << r.getScore() << "\n";
    }
}
