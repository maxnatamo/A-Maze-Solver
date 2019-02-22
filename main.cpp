#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <float.h>

#include "headers/definitions.h"
#include "headers/types.h"
#include "headers/node.h"

Size GetSize( FILE *f )
{
	Size size = { 0, 0 };
	unsigned long w, h = 0;
	unsigned char c = '\0';
	unsigned char pc = '\0';
	
	// If the file is not valid, return a size with -1.
	if( f == NULL ) { return size; }

	// Increment the width, if the current character is not a new-line.
	for( w = 0; ( c = fgetc( f ) ) && (c != '\n' && !feof( f ) ); w++ );
	rewind( f );

	while( ( c = fgetc(f) ) && !feof( f ) )
	{
		// The variable 'pc' is to check the previous character for false new-lines (lines without content).
		if( c == '\n' && pc != '\n' ) {
			h++;
		}

		pc = c;
	}
	
	size.width = w;
	size.height = h;
	
	return size;
}

int CreateGrid ( FILE *f )
{
	rewind( f );			// Rewind the current character pointer, just in case.
	nodes.reserve( size.width );	// Initialize the first vector array.

	for(int x = 0; x < size.width; x++)
	{
		nodes[x].reserve( size.height );	// Initialize each of the second dimension vector arrays.
	}

	for(int y = 0; y < size.height; y++ )
	{
		for(int x = 0; x < size.width; x++)
		{
			unsigned char c = '\n';
			CellTypes type;
		
			while( c == '\n' ) {
				c = fgetc( f );
			}

			switch( c )
			{
				case 'O':
					type = Nothing;
					break;
				case '#':
					type = Wall;
					break;
				case 'S':
					type = Start;
					break;
				case 'E':
					type = End;
					break;
				default:
					std::cout << "[ERROR] Unknown cell at " << x << ", " << y << ".";
					type = Unknown;
					break;
			}

			Node *n = new Node( x, y, type );
			nodes[x][y] = n;

			if( type == Start ) {
				startNode = n;
			} else if( type == End ) {
				endNode = n;
			}
		}
	}

	if( endNode == NULL )
	{
		std::cout << "[ERROR] No endpoint found. Please, insert a 'E' point in the maze file." << std::endl;
		return 1;
	}
	
	if( startNode == NULL )
	{
		std::cout << "[ERROR] No startpoint found. Please, insert a 'S' point in the maze file." << std::endl;
		return 1;
	}

	std::cout << "Start node at " << startNode -> x << ", " << startNode -> y << std::endl;
	std::cout << "End node at " << endNode -> x << ", " << endNode -> y << std::endl;
	
	return 0;
}

unsigned int MathAbs( unsigned int a )
{
	if( a > 0) {
		return a * -1;
	}
	return a;
}

int GetDistanceBetweenNodes( Node *a, Node *b )
{
	int dstX = MathAbs( a->x - b->x );
	int dstY = MathAbs( a->y - b->y );

	return 14 * fmin( dstX, dstY ) + 10 * ( fmax( dstX, dstY ) - fmin( dstX, dstY ) );
}

bool VectorContains( std::vector<Node *> arr, Node *n )
{
	for(int i = 0; i < arr.size(); i++)
	{
		if( arr[i] -> Equals( n ) )
			return true;
	}
	return false;
}

std::vector< Node * > RemoveFromVector( std::vector< Node *> arr, Node *n )
{
	std::vector< Node * > newArr;
	for(int i = 0; i < arr.size(); i++)
	{
		if( !arr[i]->Equals( n ) ) {
			newArr.push_back( arr[i] );
		}
	}
	return newArr;
}

void GetNodeRoute( Node *start, Node *end )
{
	std::vector< Node *> path;
	Node *n = end;

	while( !n->Equals( start ) ) {
		path.push_back( n );
		n = n->parent;
	}

	std::cout << "Route found. Node coordinates:" << std::endl;

	for(int i = 1; i < path.size(); i++)
	{
		std::cout << path[i]->x << ", " << path[i]->y << std::endl;
		nodes[ path[i] -> x ][ path[i] -> y ] -> type = Path;
	}

	std::cout << std::endl;
	PrintNodes();
}

void SolveMaze( )
{
	openCells.push_back( startNode );

	while( openCells.size() > 0 )
	{
		Node *n = openCells[0];
		for(int i = 1; i < openCells.size(); i++)
		{
			if(openCells[i]->GetFCost() < n->GetFCost() || openCells[i]->GetFCost() == n->GetFCost() && openCells[i]->hCost < n->hCost)
			{
				n = openCells[i];
			}
		}

		openCells = RemoveFromVector( openCells, n );
		closedCells.push_back( n );

		if( n->Equals( endNode ) )
		{
			GetNodeRoute( startNode, endNode );
			break;
		}

		std::vector< Node * > neighbours = n->GetNeighbours();
		for(int i = 0; i < neighbours.size(); i++)
		{
			if( neighbours[i]->type == Wall || VectorContains( closedCells, neighbours[i] ) ) {
				continue;
			}

			int moveCost = n->gCost + GetDistanceBetweenNodes( n, neighbours[i] );
			if( moveCost < neighbours[i]->gCost || ( !VectorContains( openCells, neighbours[i] ) ) ) {
				neighbours[i]->gCost = moveCost;
				neighbours[i]->hCost = GetDistanceBetweenNodes( neighbours[i], endNode );
				neighbours[i]->parent = n;

				if( !VectorContains( openCells, neighbours[i] ) ) {
					openCells.push_back( neighbours[i] );
				}
			}
		}
	}
}

void PrintNodes( )
{
	for(int y = 0; y < size.height; y++)
	{
		for(int x = 0; x < size.width; x++)
		{
			int c = nodes [x][y] -> type;
			if(c != 4) {
				if( c == 6 ) {
					std::cout << 'X';
				} else {
					std::cout << c;
				}
			} else {
				std::cout << ' ';
			}
		}
		std::cout << std::endl;
	}
}

int AStar( int argc, char *in )
{
	FILE *file = fopen( in, "r" );

        if( file != NULL )
        {
        	size = GetSize( file );
		if( CreateGrid( file ) == 0 ) {
			SolveMaze();
			return 0;
		} else {
			return 1;
		}
        }
        else
        {
        	if ( argc == 1 )
        	{
        		std::cout << "ERROR: No argument was specified." << std::endl;
        	}
        	else
        	{
        		std::cout << "ERROR: File does not exist." << std::endl;
        	}
        	return 1;
        }
	return 0;
}

int main( int argc, char *argv[] )
{
	return AStar( argc, argv[1] );
}
