#include <iostream>
#include <fstream>
#include <sstream>
#include "Movie.hpp"
#include "User.hpp"
#include "Rating.hpp"
#include "MovieManager.hpp"
#include "UserManager.hpp"
#include "RatingManager.hpp"

void loadMovies() {
    std::ifstream movieFile("data/movies.csv");
    if (!movieFile.is_open()) {
        std::cout << "movie.csv를 열 수 없습니다." << std::endl;
        return;
    }
    std::string line;
    std::getline(movieFile, line);

    while(std::getline(movieFile, line)) {
        std::istringstream ss(line);
        std::string idStr, title, genre, yearStr;
        std::getline(ss, idStr, ',');
        std::getline(ss, title, ',');
        std::getline(ss, genre, ',');
        std::getline(ss, yearStr, ',');

        int id = std::stoi(idStr);
        int year = std::stoi(yearStr);
        // Movie 객체 생성 및 MovieManager에 추가
    }
}



int main(){

}