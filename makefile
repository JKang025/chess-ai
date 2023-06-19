CXX=g++
CXXFLAGS=-g -Wall -std=c++11

OBJS = main.o board.o piece.o random.o

all: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

board.o: board.cpp board.h piece.h libconstants.h
	$(CXX) $(CXXFLAGS) -o $@ -c board.cpp

main.o: main.cpp board.h piece.h random.h libconstants.h
	$(CXX) $(CXXFLAGS) -o $@ -c main.cpp

piece.o: piece.cpp piece.h
	$(CXX) $(CXXFLAGS) -o $@ -c piece.cpp

random.o: random.cpp random.h libconstants.h
	$(CXX) $(CXXFLAGS) -o $@ -c random.cpp


clean:
	rm *.o all