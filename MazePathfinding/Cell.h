#pragma once
class Cell
{
private:

public:
	int dist;
	bool isSolid;
	bool isVisited, newIsVisited, partOfPath, newPartOfPath;
	Cell();
	~Cell();
};

