#pragma once

#include <vector>
#include "types.h"
#include "node.h"

extern Size size;
extern std::vector< std::vector< Node *> > nodes;
extern Node *startNode;
extern Node *endNode;
extern Node *currentNode;
extern std::vector< Node * > closedCells;
extern std::vector< Node * > openCells;

void PrintNodes( );
