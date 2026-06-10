
# Makefile — Movie Recommender 빌드
 
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -g
TARGET   = movie_app
 
OBJS = src/main.o          \
       src/MenuHandler.o   \
       src/Movie.o         \
       src/User.o          \
       src/Rating.o        \
       src/RatingManager.o \
       src/MovieManager.o  \
       src/UserManager.o   \
       src/BaseManager.o   \
       src/SimilarityCalc.o\
       src/Recommender.o
 
# ── 링크 ──────────────────────────────────────────────────────────────────────
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
 
# ── 컴파일 규칙 ───────────────────────────────────────────────────────────────
src/main.o: src/main.cpp src/MenuHandler.hpp src/MovieManager.hpp src/UserManager.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
src/MenuHandler.o: src/MenuHandler.cpp src/MenuHandler.hpp \
                   src/MovieManager.hpp src/UserManager.hpp \
                   src/Movie.hpp src/User.hpp src/Rating.hpp src/RatingManager.hpp \
                   src/Recommender.hpp src/Constants.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
src/Movie.o: src/Movie.cpp src/Movie.hpp src/RatingManager.hpp src/Constants.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
src/User.o: src/User.cpp src/User.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
src/Rating.o: src/Rating.cpp src/Rating.hpp src/User.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
src/RatingManager.o: src/RatingManager.cpp src/RatingManager.hpp src/Rating.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
src/MovieManager.o: src/MovieManager.cpp src/MovieManager.hpp \
                    src/Movie.hpp src/UserManager.hpp src/Constants.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
src/UserManager.o: src/UserManager.cpp src/UserManager.hpp src/User.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
src/BaseManager.o: src/BaseManager.cpp src/BaseManager.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
src/SimilarityCalc.o: src/SimilarityCalc.cpp src/SimilarityCalc.hpp \
                      src/UserManager.hpp src/MovieManager.hpp src/Constants.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
src/Recommender.o: src/Recommender.cpp src/Recommender.hpp \
                   src/UserManager.hpp src/MovieManager.hpp \
                   src/SimilarityCalc.hpp src/Constants.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


 
# ── 유틸리티 ──────────────────────────────────────────────────────────────────
.PHONY: clean run
 
run: $(TARGET)
	./$(TARGET)
 
clean:
	rm -f $(OBJS) $(TARGET)