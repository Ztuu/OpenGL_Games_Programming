#pragma once
#include "Graph.h"
#include <vector>

class Search {
public:

	Graph searchGraph; //The graph that will be searched

	Search();
	~Search();

	std::vector<int> DijkStraPath(int start, int target);
	std::vector<int> AStarPath(int start, int target);
};