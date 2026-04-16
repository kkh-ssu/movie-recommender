#include <iostream>
#include <vector>
#include "Movie.hpp"
#include "User.hpp"
#include "Rating.hpp"

int main() {

    int mode=-1;
    while(mode){
        std::cout<<"=== Movie Recommender ==="<<std::endl;
        std::cout<<"[ 영화 ]\n1. 영화추가\n2. 제목으로검색\n3. 전체목록출력\n4. 평점순정렬출력\n"<<std::endl;
        std::cout<<"[ 사용자 ]\n5. 사용자추가\n6. 사용자목록출력\n"<<std::endl;
        std::cout<<"[ 평점 ]\n7. 평점입력\n8. 영화별 평점 보기"<<std::endl;
        std::cout<<"0. 종료"<<std::endl;
        std::cout<<"선택 >";

        std::cin>>mode;

        
    }




    return 0;
}