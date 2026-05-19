#pragma once
#include "UserManager.hpp"
#include "MovieManager.hpp"
#include <map>

class SimilarityCalc {
    public:

        static double calculate(int userId1, int userId2,
                                const MovieManager& movieManager);

        static std::map<int, double> calculateAll(int targetUserId,
                                                const UserManager& userManager,
                                                const MovieManager& movieManager);
};