#include <iterator>
#include <vector>
#include <algorithm>
#include <set>
#include "maze.hh"

using namespace std;

// Remember to implement your own container here or in another cc file to store the nodes in the maze
// If you define it elsewhere, remember to make sure it's accessible by this file as well.


/**
 * @brief Find the shortest path from startNode to endNode using Djikstra's algorithm, where all path distances are 1
 * @param startNode  The node to start the search from
 * @param endNode  The node to end the search at
 * @param path  The path from startNode to endNode. If no path is found or if startNode is the same as endNode, path should be empty. The path should be in the order of the nodes visited, starting with startNode and ending with endNode
 * @return bool True if a path was found, false otherwise
 */

bool findShortestPath(Node &startNode, Node &endNode, std::vector<std::pair<int, int>>& path) {
    std::cout << "findShortestPath not implemented" << std::endl;
    return false;
    // End of student code
}
