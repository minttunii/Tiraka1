#include <iterator>
#include <vector>
#include <algorithm>
#include "maze.hh"

using namespace std;

// Remember to implement your own container here or in another cc file to store the nodes in the maze
// If you define it elsewhere, remember to make sure it's accessible by this file as well.


/**
 * @brief Connect two adjacent nodes together. If the nodes are not adjacent, do nothing
 * 
 * @param fromNode  The node to connect from
 * @param toNode  The node to connect to
 */
void connectNodes(Node& node1, Node& node2) {
    auto coord1 = node1.coords;
    auto coord2 = node2.coords;

    if(coord1.first == coord2.first){
        if(coord1.second < coord2.second){
            // Direction is above and below
            node1.paths.at("above") = &node2;
            node2.paths.at("below") = &node1;
        }
        else{
            // Direction is below and above
            node1.paths.at("below") = &node2;
            node2.paths.at("above") = &node1;
        }
    }
    else if(coord1.second == coord2.second){
        if(coord1.first < coord2.first){
            // Direction is right and left
            node1.paths.at("right") = &node2;
            node2.paths.at("left") = &node1;
        }
        else{
            // Direction is left and right
            node1.paths.at("left") = &node2;
            node2.paths.at("right") = &node1;
        }
    }
}
