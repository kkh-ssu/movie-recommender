#include "Recommender.hpp"
#include <map>
#include <algorithm>
#include <set>

std::vector<const Movie*> Recommender::recommend(int targetUserId,
                                                  const UserManager& userManager,
                                                  const MovieManager& movieManager,
                                                  int topN) {
    // 1. 타겟 유저가 이미 평가한 영화 id 수집
    std::set<int> alreadySeen;
    for (const auto& movie : movieManager.getMovies()) {
        for (const auto& r : movie.getRatingManager().getRatings()) {
            if (r.getUser().getId() == targetUserId) {
                alreadySeen.insert(movie.getId());
            }
        }
    }

    // 2. 모든 유저와 유사도 계산 후 내림차순 정렬
    auto similarityMap = SimilarityCalc::calculateAll(targetUserId,
                                                      userManager,
                                                      movieManager);
    std::vector<std::pair<int, double>> sorted(similarityMap.begin(),
                                               similarityMap.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second; // 유사도 높은 순
              });

    // 3. 유사한 유저 순서대로 안 본 영화 수집 (중복 제거)
    std::set<int>             addedMovieIds;
    std::vector<const Movie*> recommendations;

    for (const auto& [similarUserId, similarity] : sorted) {
        if (similarity <= 0) continue; // 유사도 0 이하 제외

        for (const auto& movie : movieManager.getMovies()) {
            if (alreadySeen.count(movie.getId())) continue;  // 이미 본 영화
            if (addedMovieIds.count(movie.getId())) continue; // 이미 추천한 영화

            // 해당 유저가 이 영화를 평가했는지 확인
            for (const auto& r : movie.getRatingManager().getRatings()) {
                if (r.getUser().getId() == similarUserId) {
                    recommendations.push_back(&movie);
                    addedMovieIds.insert(movie.getId());
                    break;
                }
            }

            if ((int)recommendations.size() >= topN) break;
        }
        if ((int)recommendations.size() >= topN) break;
    }

    return recommendations;
}