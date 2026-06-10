#include <cstdlib>
#include <iostream>
#include <limits>
#include "MovieManager.hpp"
#include "UserManager.hpp"
#include "MenuHandler.hpp"
 
int main() {
    MovieManager movieManager;
    UserManager  userManager;
 
    userManager.loadFromfile();
    movieManager.loadFromfile();
    movieManager.loadRatings(userManager);
 
    int mode = 0;
    while (true) {
        MenuHandler::printMenu();
        MenuHandler::printWarnings(movieManager, userManager);
 
        std::cout << "선택 > ";
        std::string line;
        std::getline(std::cin, line);
        try {
            mode = std::stoi(line);
        } catch (...) {
            mode = -1;
        }
 
        switch (mode) {
        case 1:  MenuHandler::handleAddMovie          (movieManager, userManager); break;
        case 2:  MenuHandler::handleSearchMovie       (movieManager);              break;
        case 3:  MenuHandler::handleListMovies        (movieManager);              break;
        case 4:  MenuHandler::handleListByRating      (movieManager);              break;
        case 5:  MenuHandler::handleAddUser           (userManager);               break;
        case 6:  MenuHandler::handleListUsers         (userManager);               break;
        case 7:  MenuHandler::handleAddRating         (movieManager, userManager); break;
        case 8:  MenuHandler::handleShowRatings       (movieManager);              break;
        case 9:  MenuHandler::handleRecommend         (movieManager, userManager); break;
        case 10: MenuHandler::handleGenreTopN         (movieManager);              break;
        case 11: MenuHandler::handleFilterRecommend   (movieManager, userManager); break;
        case 12: MenuHandler::handleUpdateRating      (movieManager, userManager); break;
        case 13: MenuHandler::handleShowUserRatings   (movieManager, userManager); break;
        case 0:
            std::cout << "프로그램을 종료합니다." << std::endl;
            userManager.saveToFile();
            movieManager.saveToFile();
            movieManager.saveRatings();
            return 0;
        default:
            std::cout << "잘못된 선택입니다. 다시 입력해주세요." << std::endl;
            break;
        }
 
        std::cout << "\n계속 하시려면 Enter 키를 누르세요...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        system("clear");
    }
}