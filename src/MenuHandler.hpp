#pragma once
#include "MovieManager.hpp"
#include "UserManager.hpp"
 
// 각 메뉴 항목의 입출력 처리를 담당하는 함수들.
// main()의 switch-case에서 호출되며, 비즈니스 로직은 Manager / Recommender에 위임한다.
namespace MenuHandler {
 
void printMenu();
void printWarnings    (const MovieManager& mm, const UserManager& um);
 
void handleAddMovie   (MovieManager& mm, UserManager& um);
void handleSearchMovie(MovieManager& mm);
void handleListMovies (MovieManager& mm);
void handleListByRating(MovieManager& mm);
 
void handleAddUser    (UserManager& um);
void handleListUsers  (UserManager& um);
 
void handleAddRating      (MovieManager& mm, UserManager& um);
void handleUpdateRating   (MovieManager& mm, UserManager& um); // 12. 평점 수정
void handleShowRatings    (MovieManager& mm);
void handleShowUserRatings(MovieManager& mm, UserManager& um); // 13. 내가 평가한 영화 목록
void handleRecommend      (MovieManager& mm, UserManager& um);
void handleGenreTopN      (MovieManager& mm);
void handleFilterRecommend(MovieManager& mm, UserManager& um); // 11. 연도/장르 필터 추천
 
} // namespace MenuHandler
 