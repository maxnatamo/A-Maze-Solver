CC = g++
SRC = node.cpp main.cpp definitions.cpp
OBJ = astar

all:
	$(CC) $(SRC) $(OPT) -o $(OBJ)

clean:
	rm -f $(OBJ)
