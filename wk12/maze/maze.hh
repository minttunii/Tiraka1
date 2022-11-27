#ifndef MAZE_HH
#define MAZE_HH

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <limits>


const std::string ABOVE = "above";
const std::string BELOW = "below";
const std::string LEFT = "left";
const std::string RIGHT = "right";

const int INF = std::numeric_limits<int>::max();

const int INVALID_TEST_CASE = 3;

const int NOT_FOUND = -1;


// STUDENT TODO: Add your own struct here to represent a node with a location and knowledge of its neighbours
struct Node
{
    std::pair<int, int> coords;
    std::map<std::string, Node*> paths;
};

// STUDENT TODO: Implement these functions in maze1.cc, maze2.cc, maze3.cc, and maze4.cc
Node *createNode(std::pair<int, int> loc, std::map<std::string, Node*> paths);

void connectNodes(Node& node1, Node& node2);

Node *getNeighbour(std::string direction, Node &currentNode);

bool findShortestPath(Node &startNode, Node &endNode, std::vector<std::pair<int, int>>& path);

#endif // TEST_HH

