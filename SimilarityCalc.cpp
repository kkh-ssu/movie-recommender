#include "SimilarityCalc.hpp"
#include <map>
#include <cmath>

// 유저 한 명의 평점을 { movieId → score } 맵으로 변환
static std::map<int, double> buildRatingMap(int userId,
                                            const MovieManager& movieManager) {
    std::map<int, double> ratingMap;

    for (const auto& movie : movieManager.getMovies()) {
        for (const auto& r : movie.getRatingManager().getRatings()) {
            if (r.getUser().getId() == userId) {
                ratingMap[movie.getId()] = r.getScore();
            }
        }
    }
    return ratingMap;
}

double SimilarityCalc::calculate(int userId1, int userId2,
                                 const MovieManager& movieManager) {
    auto map1 = buildRatingMap(userId1, movieManager);
    auto map2 = buildRatingMap(userId2, movieManager);

    int    commonCount    = 0;
    double scoreDiffTotal = 0.0;

    for (const auto& [movieId, score1] : map1) {
        if (map2.count(movieId)) {
            commonCount++;
            scoreDiffTotal += std::abs(score1 - map2[movieId]);
        }
    }

    if (commonCount == 0) return 0.0; // 공통 영화 없으면 유사도 0

    return (commonCount * 10) - scoreDiffTotal;
    //유사도 공식: (공통 영화 수 * 10) - 평점 차이 합
}

std::map<int, double> SimilarityCalc::calculateAll(int targetUserId,
                                                    const UserManager& userManager,
                                                    const MovieManager& movieManager) {
    std::map<int, double> result;

    for (const auto& user : userManager.getUsers()) {
        if (user.getId() == targetUserId) continue; // 자기 자신 제외
        result[user.getId()] = calculate(targetUserId, user.getId(), movieManager);
    }
    return result;
}