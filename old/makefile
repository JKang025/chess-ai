CXX=g++
CXXFLAGS=-g -Wall -std=c++11

OBJS = utils.o main.o board.o piece.o random.o 

all: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

board.o: board.h piece.h libconstants.h utils.h board.cpp 
	$(CXX) $(CXXFLAGS) -o $@ -c board.cpp

main.o: board.h piece.h random.h libconstants.h utils.h main.cpp 
	$(CXX) $(CXXFLAGS) -o $@ -c main.cpp

piece.o: piece.h piece.cpp 
	$(CXX) $(CXXFLAGS) -o $@ -c piece.cpp

random.o: random.h libconstants.h utils.h random.cpp 
	$(CXX) $(CXXFLAGS) -o $@ -c random.cpp

utils.o: utils.h libconstants.h utils.cpp 
	$(CXX) $(CXXFLAGS) -o $@ -c utils.cpp


clean:
	rm *.o all