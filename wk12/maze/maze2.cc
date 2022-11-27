#include <iterator>
#include <vector>
#include <algorithm>
#include "maze.hh"

using namespace std;

// Remember to implement your own container here or in another cc file to store the nodes in the maze
// If you define it elsewhere, remember to make sure it's accessible by this file as well.

/**
 * @brief Get a pointer to the node in the direction specified
 * 
 * @param direction  The direction to move the player
 * @param currentNode  The current node that the player is on
 * @return Node*  The new node that the player is on after the move
 */
Node *getNeighbour(std::string direction, Node &currentNode) {
    auto neighbours = currentNode.paths;
    return neighbours.at(direction);
}
