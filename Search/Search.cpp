#include "Search.h"
#include <iostream>

//Parts of this script have been replaced with pseudocode in comments
//To improve readability of the specific algorithms

Search::Search() {
	//Passed a reference to the graph to be searched
}

Search::~Search() {

}

//Calculates the path using DijkStra
std::vector<int> Search::DijkStraPath(int start, int target) {
	std::cout << "Running DijkStra" << "\n";
	std::vector<Node*> openNodes;

	//Push every node to the list of open nodes. They will be popped when they are closed
	for (int i = 0; i < searchGraph.graphNodes.size(); i++) {
		openNodes.push_back(&searchGraph.graphNodes[i]);
		if ((*openNodes[i]).index == start) {
			(*openNodes[i]).distance = 0.0; //Set start node distance to 0
		}
	}

	//While there are still nodes to search, look for the node with the shortest distance and calculate its options
	//Breaks out if it reaches the target
	while (openNodes.size() > 0) {
		
		//Find minimum node
		int minNodeIndex = 0;
		for (int i = 1; i < openNodes.size(); i++) {
			if ((*openNodes[i]).distance < (*openNodes[minNodeIndex]).distance) { minNodeIndex = i; }
		}

		Node minNode = (*openNodes[minNodeIndex]);
		openNodes.erase(openNodes.begin() + minNodeIndex);
		std::cout << "Min node is: " << minNode.index << "\n";
		
		//If the target is the node with the least distance any other path will be longer so we break
		if (minNode.index == target) { break; }

		std::vector<int> neighbourNodes;

		//Use the edges to find the indexes of any neighbouring nodes
		for (int i = 0; i < searchGraph.graphEdges.size(); i++) {
			if (searchGraph.graphEdges[i].from == minNode.index) {
				neighbourNodes.push_back(searchGraph.graphEdges[i].to);
			}
			else if (searchGraph.graphEdges[i].to == minNode.index) {
				neighbourNodes.push_back(searchGraph.graphEdges[i].from);
			}
		}

		//Find the neighbouring nodes and update their distance if it is lower than the current value
		for (int i = 0; i < openNodes.size(); i++) {
			//If this node is a neighbour
			if (std::find(neighbourNodes.begin(), neighbourNodes.end(), (*openNodes[i]).index) != neighbourNodes.end()) {
				Edge* tempEdge = searchGraph.GetEdge(minNode.index, (*openNodes[i]).index);
				if (tempEdge != nullptr) {
					double edgeLength = (*tempEdge).cost;
					double distance = minNode.distance + edgeLength;
					if (distance < (*openNodes[i]).distance) {
						(*openNodes[i]).distance = distance;
						(*openNodes[i]).previousNode = minNode.index;
					}
				}
			}
		}
	}

	Node* targetNode = searchGraph.GetNodeWithIndex(target);

	std::vector<int> result;
	std::cout << "Path: ";
	//Trace backwards from target to source and store shortest path
	//While node has a previous or we are at source
	while ((*targetNode).previousNode != 100 || (*targetNode).index == 0) {
		std::cout << "Node " << (*targetNode).index << " -> ";
		result.push_back((*targetNode).index);
		if ((*targetNode).previousNode != 100) {
			targetNode = searchGraph.GetNodeWithIndex((*targetNode).previousNode);
		}
		else {
			break;
		}
		
	}
	std::cout << "\n";
	return result;
}

//Calculates the path using A*
//Very similar process to DijkStra but uses heuristics to avoid paths that don't move towards target
std::vector<int> Search::AStarPath(int start, int target) {
	std::cout << "Running A star" << "\n";
	std::vector<Node*> openNodes;

	//Push every node to the list of open nodes. They will be popped when they are closed
	for (int i = 0; i < searchGraph.graphNodes.size(); i++) {
		openNodes.push_back(&searchGraph.graphNodes[i]);
		if ((*openNodes[i]).index == start) {
			(*openNodes[i]).distance = 0.0; //Set start node distance to 0
		}
	}

	//While there are still nodes to search, look for the node with the shortest distance+heuristic and calculate its options
	//Breaks out if it reaches the target
	while (openNodes.size() > 0) {

		//Find node with minimum distance
		int minNodeIndex = 0;
		double minDist = (*openNodes[minNodeIndex]).distance + (*openNodes[minNodeIndex]).heuristicValue;

		for (int i = 1; i < openNodes.size(); i++) {
			double newDist = (*openNodes[i]).distance + (*openNodes[i]).heuristicValue;
			
			if (newDist < minDist) { 
				minNodeIndex = i; 
				minDist = newDist;
			}
		}

		Node minNode = (*openNodes[minNodeIndex]);
		openNodes.erase(openNodes.begin() + minNodeIndex);
		std::cout << "Min node is: " << minNode.index << "\n";

		//If the target is the node with the least distance any other path will be longer so we break
		if (minNode.index == target) { break; }

		std::vector<int> neighbourNodes;

		//Use the edges to find the indexes of any neighbouring nodes
		for (int i = 0; i < searchGraph.graphEdges.size(); i++) {
			if (searchGraph.graphEdges[i].from == minNode.index) {
				neighbourNodes.push_back(searchGraph.graphEdges[i].to);
			}
			else if (searchGraph.graphEdges[i].to == minNode.index) {
				neighbourNodes.push_back(searchGraph.graphEdges[i].from);
			}
		}

		//Find the neighbouring nodes and update their distance if it is lower than the current value
		for (int i = 0; i < openNodes.size(); i++) {
			//If this node is a neighbour
			if (std::find(neighbourNodes.begin(), neighbourNodes.end(), (*openNodes[i]).index) != neighbourNodes.end()) {
				double edgeLength = 1000.0f;
				Edge* tempEdge = searchGraph.GetEdge(minNode.index, (*openNodes[i]).index);
				if (tempEdge != nullptr) {
					edgeLength = (*tempEdge).cost;
				}

				double distance = minNode.distance + edgeLength;
				if (distance < (*openNodes[i]).distance) {
					(*openNodes[i]).distance = distance;
					(*openNodes[i]).previousNode = minNode.index;
				}
			}
		}
	}

	Node* targetNode = searchGraph.GetNodeWithIndex(target);

	std::vector<int> result;

	//Trace backwards from target to source and store shortest path
	//While node has a previous or we are at source
	while ((*targetNode).previousNode != 100 || (*targetNode).index == 0) {
		std::cout << "Node " << (*targetNode).index << "\n";
		result.push_back((*targetNode).index);
		if ((*targetNode).previousNode != 100) {
			targetNode = searchGraph.GetNodeWithIndex((*targetNode).previousNode);
		}
		else {
			break;
		}

	}

	return result;
}