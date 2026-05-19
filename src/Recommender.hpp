#pragma once
#include "UserManager.hpp"
#include "MovieManager.hpp"
#include "SimilarityCalc.hpp"
#include <vector>

class Recommender {
public:
    // targetUser가 보지 않은 영화 중 추천 목록 반환
    static std::vector<const Movie*> recommend(int targetUserId,
                                               const UserManager& userManager,
                                               const MovieManager& movieManager,
                                               int topN = 5);
};