#pragma once

#include <vector>
#include <math.h>

#include "types.h"

class Node
{
	public:
	int x = -1;
	int y = -1;
	int fCost, gCost, hCost;

	CellTypes type;
	Node *parent;

	public:
	// Constructer
	Node( int, int, CellTypes );

	int GetFCost ( );

	// Get all surrounding neighbours and assign them to the 'neighbours' variable.
	std::vector< Node * > GetNeighbours( );

	bool Equals( Node * );
};
