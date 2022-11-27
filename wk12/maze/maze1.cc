#include <iterator>
#include <vector>
#include <algorithm>
#include <set>
#include "maze.hh"

using namespace std;

// Remember to implement your own container here or in another cc file to store the nodes in the maze
// If you define it elsewhere, remember to make sure it's accessible by this file as well.
std::map<std::pair<int, int>, Node*> maze_nodes;

/**
 * @brief Create a Node object
 * 
 * @param loc  The location of the node
 * @return Node*  A pointer to the node that is created. If the node already exists, return a pointer to the existing node
 */
Node* createNode  (std::pair<int, int> loc, std::map<std::string, Node*> paths) {
    auto it = maze_nodes.find(loc);

    if(it != maze_nodes.end()){
        auto node_ptr = it->second;
        return node_ptr;
    }

    Node newNode;
    newNode.coords = loc;
    newNode.paths = paths;
    auto it2 = maze_nodes.insert({loc, &newNode});

    return it2.first->second;
}
