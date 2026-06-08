#include "SimilarityCalc.hpp"
#include <cmath>

// ── calculate ─────────────────────────────────────────────────────────────────
// 기존: buildRatingMap()이 O(M×R) 이중 루프로 맵을 새로 생성
// 개선: getUserRatings()로 O(1) 조회 — 복사 없이 const 참조 사용
double SimilarityCalc::calculate(int userId1, int userId2,
                                 const MovieManager& movieManager) {
    const auto& map1 = movieManager.getUserRatings(userId1); // O(1)
    const auto& map2 = movieManager.getUserRatings(userId2); // O(1)

    int    commonCount    = 0;
    double scoreDiffTotal = 0.0;

    // map1을 순회하며 map2에 같은 movieId가 있으면 공통 영화
    for (const auto& [movieId, score1] : map1) {
        auto it = map2.find(movieId); // O(1)
        if (it != map2.end()) {
            commonCount++;
            scoreDiffTotal += std::abs(score1 - it->second);
        }
    }

    if (commonCount == 0) return 0.0;
    return (commonCount * 10) - scoreDiffTotal;
}

// ── calculateAll ──────────────────────────────────────────────────────────────
// 변경 없음 — calculate()가 빨라졌으므로 전체도 자동으로 개선됨
std::map<int, double> SimilarityCalc::calculateAll(int targetUserId,
                                                    const UserManager& userManager,
                                                    const MovieManager& movieManager) {
    std::map<int, double> result;

    for (const auto& user : userManager.getUsers()) {
        if (user.getId() == targetUserId) continue;
        result[user.getId()] = calculate(targetUserId, user.getId(), movieManager);
    }
    return result;
}
