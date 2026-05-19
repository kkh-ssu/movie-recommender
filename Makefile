# Makefile — Movie 클래스 빌드
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -g
TARGET   = movie_app
OBJS = main.o Movie.o User.o Rating.o MovieManager.o UserManager.o RatingManager.o SimilarityCalc.o Recommender.o


$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp Movie.hpp User.hpp Rating.hpp
	$(CXX) $(CXXFLAGS) -c $<

Movie.o: Movie.cpp Movie.hpp
	$(CXX) $(CXXFLAGS) -c $<

User.o: User.cpp User.hpp
	$(CXX) $(CXXFLAGS) -c $<

Rating.o: Rating.cpp Rating.hpp
	$(CXX) $(CXXFLAGS) -c $<	

MovieManager.o: MovieManager.cpp MovieManager.hpp Movie.hpp
	$(CXX) $(CXXFLAGS) -c $<

UserManager.o: UserManager.cpp UserManager.hpp User.hpp
	$(CXX) $(CXXFLAGS) -c $<

RatingManager.o: RatingManager.cpp RatingManager.hpp Rating.hpp
	$(CXX) $(CXXFLAGS) -c $<

SimilarityCalc.o: SimilarityCalc.cpp SimilarityCalc.hpp
	$(CXX) $(CXXFLAGS) -c $<

Recommender.o: Recommender.cpp Recommender.hpp
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean run
clean:
	rm -f $(OBJS) $(TARGET)
run: $(TARGET)
	./$(TARGET)
