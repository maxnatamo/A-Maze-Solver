#include <vector>
#include "headers/types.h"
#include "headers/definitions.h"

Size size;
std::vector< std::vector< Node *> > nodes;
Node *startNode;
Node *endNode;
Node *currentNode;
std::vector< Node * > closedCells;
std::vector< Node * > openCells;
