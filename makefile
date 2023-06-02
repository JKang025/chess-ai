CXX=g++
CXXFLAGS=-g -Wall -std=c++11

OBJS = main.o board.o piece.o 

all: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

board.o: board.cpp board.h piece.h
	$(CXX) $(CXXFLAGS) -o $@ -c board.cpp

main.o: main.cpp board.h piece.h
	$(CXX) $(CXXFLAGS) -o $@ -c main.cpp

piece.o: piece.cpp piece.h
	$(CXX) $(CXXFLAGS) -o $@ -c piece.cpp


clean:
	rm *.o all