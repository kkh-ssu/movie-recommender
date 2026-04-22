#include <iostream>
#include <vector>
#include <cstdlib>
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
        std::cout<<"=== Movie Recommender ===\n"<<std::endl;
        std::cout<<"[ 영화 ]\n1. 영화 추가\n2. 제목으로 검색\n3. 전체 목록 출력\n4. 평점순 출력\n"<<std::endl;
        std::cout<<"[ 사용자 ]\n5. 사용자 추가\n6. 사용자 목록 출력\n"<<std::endl;
        std::cout<<"[ 평점 ]\n7. 평점 입력\n8. 영화별 평점 보기\n"<<std::endl;
        std::cout<<"0. 종료\n"<<std::endl;

        if(movieManager.getMovieCount()==0){
            std::cout<<"경고: 등록된 영화가 없습니다. 영화를 추가해주세요.\n"<<std::endl;
        }
        if(userManager.getUserCount()==0){
            std::cout<<"경고: 등록된 사용자가 없습니다. 사용자를 추가해주세요.\n"<<std::endl;
        }

        std::cout<<"선택 >";
        std::cin>>mode;

        switch (mode) {
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
            if (movieManager.alreadyExists(title,genre,year)){
                std::cout<<"이미 존재하는 영화입니다."<<std::endl;
                break;
            }
            else{
            movieManager.addMovie(Movie(movieManager.getMovieCount() + 1, title, genre, year));
            std::cout<<"영화가 추가되었습니다."<<std::endl;
            }
            break;
        }
        case 2:{
            std::cout<<"검색 할 영화 제목을 입력해주세요 : ";
            std::string title;
            std::cin>>title; //todo 제목이 같을 때 출시년도, 장르 물어보기
            const Movie* found=movieManager.findMovieByTitle(title);
            if (found){
                std::cout<<*found<<std::endl;
            }
            else{
                std::cout<<"영화를 찾을 수 없습니다."<<std::endl;
            }
            break;
        }
        case 3:{
            if (movieManager.getMovieCount()==0){
                std::cout<<"등록된 영화가 없습니다."<<std::endl;
                break;
            }
            else{
                movieManager.displayMovies();
                break;
            }
        }
        case 4: {
            if (movieManager.getMovieCount() == 0) {
                std::cout << "등록된 영화가 없습니다." << std::endl;
                break;
            }
            else {
                movieManager.displaySortedByRating();
                break;
            }
        }
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
        case 6:{
            if (userManager.getUserCount()==0){
                std::cout<<"등록된 사용자가 없습니다."<<std::endl;
                break;
            }
            else{
                userManager.displayUsers();
                // 사용자 목록 출력 로직
                break;
            }
        }
        case 7:{
            std::cout<<"평점을 입력할 영화 제목을 입력해주세요 : ";
            std::string title;
            std::cin>>title;
            Movie* movie=movieManager.findMovieByTitle(title);
            if (movie){
                std::cout<<movie->getTitle()<<"의 평점을 입력합니다"<<std::endl;
                std::cout<<"유저 목록"<<std::endl;
                userManager.displayUsers();
                std::cout<<"평점을 입력할 사용자 이름을 입력해주세요 : ";
                std::string name;
                std::cin>>name;
                if(userManager.validUser(name)){
                    std::cout<<"평점을 입력해주세요 (0.0 ~ 5.0) : ";
                    double score;
                    std::cin>>score;
                    const User* user=userManager.findUserByName(name);
                    if (score>=0.0 && score<=5.0){
                        movie->getRatingManager().addRating(Rating(*user, score));
                        std::cout<<"평점이 입력되었습니다."<<std::endl;
                    }
                    else{
                        std::cout<<"잘못된 평점입니다."<<std::endl;
                    }
                }
                else{
                    std::cout<<"사용자를 찾을 수 없습니다."<<std::endl;
                }
            }
            else{
                std::cout<<"영화를 찾을 수 없습니다."<<std::endl;
            }
            // 평점 입력 로직
            break;
        }
        case 8:{
            // 영화별 평점 보기 로직
            std::cout<<"평점을 볼 영화 제목을 입력해주세요 : ";
            std::string title;
            std::cin>>title;
            const Movie* movie=movieManager.findMovieByTitle(title);
            if (movie){
                std::cout<<movie->getTitle()<<"의 평점 목록"<<std::endl;
                movie->getRatingManager().displayRatings();
            }
            else{
                std::cout<<"영화를 찾을 수 없습니다."<<std::endl;
            }
            break;

        }
        case 0:
            std::cout << "프로그램을 종료합니다." << std::endl;
            return 0;
        default:
            std::cout << "잘못된 선택입니다. 다시 입력해주세요." << std::endl;
            break;
        }
        std::cout<<std::endl;
        std::cout<<"계속 하시려면 Enter 키를 누르세요...";
        std::cin.ignore();
        std::cin.get();
        system("clear");
    }

    return 0;
}