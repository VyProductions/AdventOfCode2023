PROJECT=puzzle12

CXX=g++
CXXFLAGS=-std=c++2a -Wall -Wextra -pedantic -Wshadow -Wconversion -Werror=vla -Werror=return-type -Wno-deprecated-copy -Wno-multichar -g

all: clean puzzleA puzzleB
	@echo "Running $(PROJECT) A..."
	@./puzzleA
	@echo "Running $(PROJECT) B..."
	@./puzzleB

puzzleA: puzzleA.o
	@$(CXX) $(CXXFLAGS) -o puzzleA puzzleA.o

puzzleB: puzzleB.o
	@$(CXX) $(CXXFLAGS) -o puzzleB puzzleB.o

puzzleA.o:
	@$(CXX) $(CXXFLAGS) -c $(PROJECT)A.cpp
	@mv $(PROJECT)A.o puzzleA.o

puzzleB.o:
	@$(CXX) $(CXXFLAGS) -c $(PROJECT)B.cpp
	@mv $(PROJECT)B.o puzzleB.o

clean:
	@rm -f *.o puzzleA puzzleB
