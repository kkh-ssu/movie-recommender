# Makefile — Movie 클래스 빌드
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -g
TARGET   = movie_app
OBJS = src/main.o src/Movie.o src/User.o src/Rating.o src/MovieManager.o src/UserManager.o src/RatingManager.o src/BaseManager.o src/SimilarityCalc.o src/Recommender.o



$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/main.o: src/main.cpp src/Movie.hpp src/User.hpp src/Rating.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/Movie.o: src/Movie.cpp src/Movie.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/User.o: src/User.cpp src/User.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/Rating.o: src/Rating.cpp src/Rating.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/MovieManager.o: src/MovieManager.cpp src/MovieManager.hpp src/Movie.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/UserManager.o: src/UserManager.cpp src/UserManager.hpp src/User.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/RatingManager.o: src/RatingManager.cpp src/RatingManager.hpp src/Rating.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/BaseManager.o: src/BaseManager.cpp src/BaseManager.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/SimilarityCalc.o: src/SimilarityCalc.cpp src/SimilarityCalc.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/Recommender.o: src/Recommender.cpp src/Recommender.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run
clean:
	rm -f $(OBJS) $(TARGET)
run: $(TARGET)
	./$(TARGET)
