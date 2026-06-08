#include "Recommender.hpp"
#include <map>
#include <algorithm>
#include <set>

std::vector<const Movie*> Recommender::recommend(int targetUserId,
                                                  const UserManager& userManager,
                                                  const MovieManager& movieManager,
                                                  int topN) {
    // 기존: O(M×R) 이중 루프로 평점 수 확인
    // 개선: O(1) 인덱스 조회
    if (movieManager.getUserRatingCount(targetUserId) == 0) {
        return {};
    }

    // 기존: 전체 영화 순회하며 targetUser 평점 찾아 alreadySeen 구축 — O(M×R)
    // 개선: getUserRatings()로 O(1) 조회
    std::set<int> alreadySeen;
    for (const auto& [movieId, score] : movieManager.getUserRatings(targetUserId)) {
        alreadySeen.insert(movieId);
    }

    // 유사도 계산 후 내림차순 정렬 (변경 없음 — calculate() 자체가 빨라짐)
    auto similarityMap = SimilarityCalc::calculateAll(targetUserId,
                                                      userManager,
                                                      movieManager);
    std::vector<std::pair<int, double>> sorted(similarityMap.begin(),
                                               similarityMap.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });

    // 유사 유저 순서로 안 본 영화 수집
    // 기존: movie.getRatingManager().getRatings() 순회 — O(M×R) per 유저
    // 개선: getUserRatings(similarUserId)로 O(1) 조회 후 movieId set으로 확인
    std::set<int>             addedMovieIds;
    std::vector<const Movie*> recommendations;

    for (const auto& [similarUserId, similarity] : sorted) {
        if (similarity <= 0) continue;

        const auto& similarUserRatings =
            movieManager.getUserRatings(similarUserId); // O(1)

        for (const auto& [movieId, score] : similarUserRatings) {
            if (alreadySeen.count(movieId))   continue; // 이미 본 영화
            if (addedMovieIds.count(movieId)) continue; // 이미 추천한 영화

            // movieId로 Movie* 조회 — O(1)
            const Movie* movie = movieManager.findMovieById(movieId);
            if (!movie) continue;

            recommendations.push_back(movie);
            addedMovieIds.insert(movieId);

            if (static_cast<int>(recommendations.size()) >= topN) break;
        }
        if (static_cast<int>(recommendations.size()) >= topN) break;
    }

    return recommendations;
}
