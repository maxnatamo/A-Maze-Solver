#include <iostream>
#include <math.h>

#include "headers/node.h"
#include "headers/types.h"
#include "headers/definitions.h"

Node::Node( int _x, int _y, CellTypes _type )
{
	x = _x;
	y = _y;
	type = _type;
}

int Node::GetFCost( )
{
	return hCost + gCost;
}

std::vector<Node *> Node::GetNeighbours()
{
	std::vector< Node * > neighbours;
	for(int nY = y - 1; nY <= y + 1; nY++)
	{
		for(int nX = x - 1; nX <= x + 1; nX++)
		{
			if( nX >= 0 && nY >= 0 && nX < size.width && nY < size.height )
			{
				if( !(nX == x && nY == y) )
				{
					if( nodes[nX][nY]->type != Wall )
					{
						neighbours.push_back( nodes[nX][nY] );
					}
				}
			}
		}
	}
	return neighbours;
}

bool Node::Equals( Node *n )
{
	return n->x == x && n->y == y;
}
