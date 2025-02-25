OPTIONS = -g -Wall
PROGRAM = minimax.out
CXX = g++

all: $(PROGRAM)

run: $(PROGRAM)
	./$(PROGRAM)

$(PROGRAM): minimax.cpp
	$(CXX) $(OPTIONS) minimax.cpp -o $(PROGRAM) 

clean:
	rm -f $(PROGRAM)

