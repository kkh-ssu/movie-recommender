#pragma once
#include "UserManager.hpp"
#include "MovieManager.hpp"
#include "SimilarityCalc.hpp"
#include <vector>

class Recommender {
public:
    // 평점 3개 미만 유저: 평균 평점 상위 3개 추천
    // 그 외: 유사도 높은 유저의 상위 평점 2개씩 topN개 추천
    static std::vector<const Movie*> recommend(int targetUserId,
                                               const UserManager& userManager,
                                               const MovieManager& movieManager,
                                               int topN = 5);

private:
    // 평점 이력이 적은 유저용: 전체 평균 평점 상위 N개
    static std::vector<const Movie*> recommendByAverage(
                                               const MovieManager& movieManager,
                                               int topN = 3);
};
