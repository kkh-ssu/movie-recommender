#include <iostream>
#include <string>

int main(void){
	std::string name;

	std::cout<<"=== 영화 추천 시스템===\n";
	std::cout<<"이름 입력: ";
	getline(std::cin,name);
	std::cout<<name<<"님 환영합니다\n";
	return 0;
}
