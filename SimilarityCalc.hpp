#pragma once
#include "UserManager.hpp"
#include "MovieManager.hpp"
#include <map>

class SimilarityCalc {
public:
    // userId 두 개를 받아 유사도 반환
    static double calculate(int userId1, int userId2,
                            const MovieManager& movieManager);

    // userId 기준으로 모든 유저와의 유사도 계산
    // { userId → 유사도 } 맵 반환
    static std::map<int, double> calculateAll(int targetUserId,
                                              const UserManager& userManager,
                                              const MovieManager& movieManager);
};