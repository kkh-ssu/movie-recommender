#include "Recommender.hpp"
#include "Constants.hpp"
#include <map>
#include <algorithm>
#include <set>
#include <iostream>
 
// ── recommendByAverage ────────────────────────────────────────────────────────
// 평점 MIN_RATING_THRESHOLD개 미만 유저용 폴백.
// 전체 영화를 평균 평점 내림차순으로 정렬해 상위 topN개를 반환한다.
// 평점이 하나도 없는 영화(averageRating == 0)는 제외한다.
std::vector<const Movie*> Recommender::recommendByAverage(
        const MovieManager& movieManager, int topN) {
 
    std::vector<const Movie*> candidates;
    for (const auto& movie : movieManager.getMovies()) {
        if (movie.getAverageRating() > MIN_SCORE)
            candidates.push_back(&movie);
    }
 
    std::sort(candidates.begin(), candidates.end(),
              [](const Movie* a, const Movie* b) {
                  return a->getAverageRating() > b->getAverageRating();
              });
 
    if (static_cast<int>(candidates.size()) > topN)
        candidates.resize(topN);
 
    return candidates;
}

std::set<int> Recommender::buildAlreadySeenSet(int targetUserId, const MovieManager &movieManager)
{
    std::set<int> alreadySeen;
    for (const auto& [movieId, score] : movieManager.getUserRatings(targetUserId))
        alreadySeen.insert(movieId);
    return alreadySeen;
}

// ── recommend ─────────────────────────────────────────────────────────────────
std::vector<const Movie*> Recommender::recommend(int targetUserId,
                                                  const UserManager& userManager,
                                                  const MovieManager& movieManager,
                                                  int topN) {
    int ratingCount = movieManager.getUserRatingCount(targetUserId);
 
    // 평점 MIN_RATING_THRESHOLD개 미만 → 평균 평점 상위 DEFAULT_AVERAGE_RECOMMEND_COUNT개 추천
    if (ratingCount < MIN_RATING_THRESHOLD) {
        std::cout << "평점이 없거나 적은 유저의 경우 "
                     "평균 평점이 가장 높은 영화들을 추천합니다\n";
        return recommendByAverage(movieManager, DEFAULT_AVERAGE_RECOMMEND_COUNT);
    }
 
    // 대상 유저가 이미 본 영화 수집
    std::set<int> alreadySeen = buildAlreadySeenSet(targetUserId, movieManager);
 
    // 모든 유저와 유사도 계산 후 내림차순 정렬
    auto similarityMap = SimilarityCalc::calculateAll(targetUserId,
                                                      userManager,
                                                      movieManager);
    std::vector<std::pair<int, double>> sorted(similarityMap.begin(),
                                               similarityMap.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });
 
    // 유사도 높은 유저 순서대로 상위 평점 MAX_PICKS_PER_USER개씩 topN까지 수집
    std::set<int>             addedMovieIds;
    std::vector<const Movie*> recommendations;
 
    for (const auto& [similarUserId, similarity] : sorted) {
        if (similarity <= 0) continue;
 
        // 해당 유저의 평점 맵을 score 내림차순으로 정렬
        const auto& ratings = movieManager.getUserRatings(similarUserId);
 
        std::vector<std::pair<int, double>> sortedRatings(ratings.begin(),
                                                          ratings.end());
        std::sort(sortedRatings.begin(), sortedRatings.end(),
                  [](const auto& a, const auto& b) {
                      return a.second > b.second; // 높은 평점 먼저
                  });
 
        // 상위 평점 MAX_PICKS_PER_USER개만 추천 후보로 사용
        int pickedFromThisUser = 0;
        for (const auto& [movieId, score] : sortedRatings) {
            if (pickedFromThisUser >= MAX_PICKS_PER_USER) break;
            if (alreadySeen.count(movieId))   continue;
            if (addedMovieIds.count(movieId)) continue;
 
            const Movie* movie = movieManager.findMovieById(movieId);
            if (!movie) continue;
 
            recommendations.push_back(movie);
            addedMovieIds.insert(movieId);
            pickedFromThisUser++;
 
            if (static_cast<int>(recommendations.size()) >= topN) break;
        }
 
        if (static_cast<int>(recommendations.size()) >= topN) break;
    }
 
    return recommendations;
}