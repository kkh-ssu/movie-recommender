#pragma once
#include "UserManager.hpp"
#include "MovieManager.hpp"
#include "SimilarityCalc.hpp"
#include "Constants.hpp"
#include <vector>
#include <set>
 
class Recommender {
public:
    // 평점 MIN_RATING_THRESHOLD개 미만 유저: 평균 평점 상위 DEFAULT_AVERAGE_RECOMMEND_COUNT개 추천
    // 그 외: 유사도 높은 유저의 상위 평점 MAX_PICKS_PER_USER개씩 topN개 추천
    static std::vector<const Movie*> recommend(int targetUserId,
                                               const UserManager& userManager,
                                               const MovieManager& movieManager,
                                               int topN = DEFAULT_TOP_N);
 
private:
    // 평점 이력이 적은 유저용: 전체 평균 평점 상위 N개
    static std::vector<const Movie*> recommendByAverage(
                                               const MovieManager& movieManager,
                                               int topN = DEFAULT_AVERAGE_RECOMMEND_COUNT);

    static std::set<int> buildAlreadySeenSet(int targetUserId, const MovieManager& movieManager);
};
 