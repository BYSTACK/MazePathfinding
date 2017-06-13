#include "Cell.h"
#include <climits>


Cell::Cell()
{
	dist = INT_MAX;
	isVisited = false;
	newIsVisited = false;
	partOfPath = false;
	newPartOfPath = false;
}


Cell::~Cell()
{
}
