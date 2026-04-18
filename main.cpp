#include <iostream>
#include <vector>
#include "Movie.hpp"
#include "User.hpp"
#include "Rating.hpp"
#include "MovieManager.hpp"
#include "UserManager.hpp"
#include "RatingManager.hpp"

int main() {
    MovieManager movieManager;
    UserManager userManager;
    int mode=-1;
    while(mode){
        std::cout<<"=== Movie Recommender ==="<<std::endl;
        std::cout<<"[ 영화 ]\n1. 영화추가\n2. 제목으로검색\n3. 전체목록출력\n4. 평점순정렬출력\n"<<std::endl;
        std::cout<<"[ 사용자 ]\n5. 사용자추가\n6. 사용자목록출력\n"<<std::endl;
        std::cout<<"[ 평점 ]\n7. 평점입력\n8. 영화별 평점 보기\n"<<std::endl;
        std::cout<<"0. 종료"<<std::endl;
        std::cout<<"선택 >";

        std::cin>>mode;

        switch (mode)
        {
        case 1 : {
            std::cout<<"추가 할 영화 제목을 입력해주세요 : ";
            std::string title;
            std::cin>>title;
            std::cout<<"추가 할 영화 장르를 입력해주세요 : ";
            std::string genre;
            std::cin>>genre;
            std::cout<<"추가 할 영화 출시 연도를 입력해주세요 : ";
            int year;
            std::cin>>year;
            movieManager.addMovie(Movie(movieManager.getMovieCount() + 1, title, genre, year));
            // 영화 추가 로직
            break;
        }
        case 2:{
            // 제목으로 검색 로직
            break;
        }
        case 3:
            // 전체 목록 출력 로직
            break;
        case 4:
            // 평점 순 정렬 출력 로직
            break;
        case 5:{
            std::cout<<"추가 할 사용자 이름을 입력해주세요 : ";
            std::string name;
            std::cin>>name;
            std::cout<<"추가 할 사용자 이메일을 입력해주세요 : ";
            std::string email;
            std::cin>>email;
            if (userManager.validUser(name,email)){
                std::cout<<"이미 존재하는 사용자입니다."<<std::endl;
            }
            else{
                userManager.addUser(User(userManager.getUserCount() + 1, name, email));
                std::cout<<"사용자가 추가되었습니다."<<std::endl;
            }
            break;
        }
        case 6:
            // 사용자 목록 출력 로직
            break;
        case 7:
            // 평점 입력 로직
            break;
        case 8:
            // 영화별 평점 보기 로직
            break;
        case 0:
            std::cout << "프로그램을 종료합니다." << std::endl;
            break;
        default:
            std::cout << "잘못된 선택입니다. 다시 입력해주세요." << std::endl;
            break;
        }
    }

    return 0;
}