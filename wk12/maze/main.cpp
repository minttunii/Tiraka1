//#include <QCoreApplication>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>

#include <string>
#include "maze.hh"




void usage()
{
    std::cout << "=============================================" << std::endl;
    std::cout << "Usage: maze <test_function> <test_num>" << std::endl;
    std::cout << "test_function: 1, 2, 3, 4, 5, 6 (number of the exercise as in 1->maze1.cc, 2->maze2.cc, etc..)" << std::endl;
    std::cout << "test_num: 1, 2, 3, 4, 5 (number of the test as in 1->test case 1 for maze[x].cc, 2-> test case 2 for maze[x].cc, etc..)" << std::endl;
    // Example
    std::cout << "Example: maze 1 1 (test createNode (maze1.cc) with the first test case)" << std::endl;
    std::cout << "=============================================" << std::endl;
}

std::map<std::string, bool> createPaths (std::vector<std::string> availablePaths) {
  std::map<std::string, bool> paths{{"left", false}, {"above", false}, {"right", false}, {"below", false}};
  for(std::string path : availablePaths)
    paths.at(path) = true;
  // return paths
  return paths;
}

/**
 * @brief prints the vector
 * 
 * @param v  vector to print
 */
void printVector(const std::vector<int>& v)
{
  std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl << std::endl;
}

/**
 * @brief prints the vector of pairs
 * 
 * @param v  vector of pairs to print
 */
void printVectorOfPairs(const std::vector<std::pair<int, int>>& v)
{
  for (auto& i : v)
    std::cout << "(" << i.first << ", " << i.second << ") ";
  std::cout << std::endl << std::endl;
}


/**
 * @brief Prints the available paths of a node
 * 
 * @param m  - the map of paths to be printed
 */
void printPathMap(const std::map<std::string, bool>& m)
{
  std::cout << "{ ";
  for (auto it = m.begin(); it != m.end(); ++it)
  {
    std::cout << "{" << it->first << ", " << it->second << "} ";
    if(std::next(it) != m.end())
      std::cout << ", ";
  }
  std::cout << " }" << std::endl;
}

/**
 * @brief Prints the description of the test case
 * 
 * @param description 
 */
void printDescription(std::string &description)
{
    std::cout << "Description: " << description << std::endl;
}

void printPair(std::pair<int, int> &p)
{
    std::cout << "Pair: (" << p.first << ", " << p.second << ")" << std::endl;
}

/**
 * @brief Prints a description of the test case and the result of the test
 * 
 * @param returnLoc   - the location of the node returned by the function
 * @param expectedLoc   - the expected location of the node
 */
void printLocEquality(std::pair<int, int> returnLoc, std::pair<int, int> expectedLoc) {
    std::string description = "Check if getNeighbour returns a node with the correct location";
    printDescription(description);
    std::cout << "EXPECTED VALUE: " << std::endl;
    printPair(expectedLoc);
    std::cout << "RETURNED VALUE: " << std::endl;
    printPair(returnLoc);

    if (expectedLoc == returnLoc)
    {
      std::cout << "SUCCESS: Returned pointer with correct coordinates" <<  std::endl;
    }
    else {
      std::cout << "FAILURE: Returned pointer with incorrect coordinates " << std::endl;
    }
}

/**
 * @brief Prints the setup of the test case
 * 
 * @param firstNodeLoc  - the location of the first node
 * @param direction   - the direction that we are going to move
 * @param expectedLoc  - the expected location of the node we find after moving to the direction
 */
void printSetup(std::pair<int, int> firstNodeLoc, std::string direction, std::pair<int, int> expectedLoc) {
    std::cout << "SETUP: start location:{" << firstNodeLoc.first << ", " << firstNodeLoc.second << "}" << std::endl;
    std::cout << "SETUP: testDirection:" << direction << std::endl;
    std::cout << "SETUP: expected location after getNeighbour:{" << expectedLoc.first << ", " << expectedLoc.second << "}" << std::endl;
}

/**
 * @brief Prints the expected and returned values of the test case
 * 
 * @param expected   - the expected path 
 * @param returned   - the returned path 
 */
void printExpectedAndReturnedPath(std::map<std::string, bool> expected, std::map<std::string, bool> returned) {
  std::cout << "EXPECTED VALUE:" << std::endl;
  printPathMap(expected);
  std::cout << "RETURNED VALUE:" << std::endl;
  printPathMap(returned);
}

/**
 * @brief makes sure the getNeighbour function has returned a node that is not null
 * 
 * @param Node  - the id of the test case
 * @return int  - 0 if the test case passed, 1 otherwise
 */
int nodeCantBeNull(Node* node) {
  if (node == nullptr) {
    std::cout << "FAILURE: getNeighbour returned a null pointer. You need to implement createNode correctly first. " << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/** 
 * Create and connect nodes
 * @brief Creates nodes from the given vector of vectors of strings
 * @param vector of vectors of strings: options are node, path, and wall. Every node can have at most 4 accessible paths, and every path has 2 nodes. Every vector of strings with an even number of index shows the accessibility of the paths of the nodes vertically, and every vector of strings with an odd number of index shows the nodes at odd index and at even index shows whether or not the nodes are connected horizontally.
 * @example If the vector is {{node, path, node}, {wall, wall, path}, {node, path, node}}:
 *        - the first (0, 0) node will have a path to the node to its right (1, 0)
 * 
 * 
 * 
 * @return first and last node of the maze
 * 
*/



/**
 * @brief tests the createNode function
 * 
 * @param test_id  - the id of the test case
 * @return int - EXIT_SUCCESS if the test_id is valid, INVALID_TEST_CASE if the test_id is invalid
 */
int testCreateNode(int test_id) {
  std:: cout << "TEST " << test_id << ": createNode() " << std::endl;
  // Vector of vectors of integers of 0 to 1
  

  std::pair<int,int> loc= {2,2};
  std::map<std::string, Node*> noAccessiblePaths{}; // equals:  {"left", nullptr}, {"above", nullptr}, {"right", nullptr}, {"below", nullptr}
  switch (test_id)
  {
  case 1:
   {
    // Test that the Node is created successfully
    std::string description = "Test that the Node is created successfully, and that createNode returns a Node pointer";
    printDescription(description);
    
    auto node = createNode(loc, noAccessiblePaths);
    // Make sure that node is a Node pointer
    if (typeid(node) != typeid(Node*)) {
      std::cout << "FAILURE: node is not a Node pointer" << std::endl;
    }
    else {
      std::cout << "SUCCESS: node is a Node pointer" << std::endl;
    }
    break;
  }
    case 2:
   {
    // Test that the Node is created successfully
    std::string description = "Make sure that upon a successful creation of a Node, the returned node is not a nullptr";
    printDescription(description);
    auto node = createNode(loc, noAccessiblePaths);
    std::map<std::string, Node*> paths{};
    // Make sure that node is not a nullptr
    if (node == nullptr) {
      std::cout << "FAILURE: node is a nullptr" << std::endl;
    }
    else {
      std::cout << "SUCCESS: node is not a nullptr" << std::endl;
    }
    break;
  }
  case 3:
   {
    // Test that the Node is created successfully
    std::string description = "Make sure you can create multiple nodes with different locations";
    printDescription(description);
    std::pair<int, int> loc1 = {2,2};
    std::pair<int, int> loc2 = {3,3};

    auto node1 = createNode(loc1, noAccessiblePaths);
    auto node2 = createNode(loc2, noAccessiblePaths);
    // Make sure that node is not a nullptr
    if (node1 == nullptr || node2 == nullptr) {
      std::cout << "FAILURE: node is a nullptr" << std::endl;
    }
    else {
    // Make sure that node is a Node pointer
    if (typeid(node1) != typeid(Node*)) {
      std::cout << "FAILURE: node1 is not a Node pointer" << std::endl;
    }
    else {
      std::cout << "SUCCESS: node1 is a Node pointer" << std::endl;
    }
    if (typeid(node2) != typeid(Node*)) {
      std::cout << "FAILURE: node2 is not a Node pointer" << std::endl;
    }
    else {
      std::cout << "SUCCESS: node2 is a Node pointer" << std::endl;
    }
    }
    break;
  }
  case 4:
   {
    // Test that the Node is created successfully
    std::string description = "If a node with the same location already exists, createNode should return the existing node";
    printDescription(description);
    std::pair<int, int> loc = {2,2};


    auto existingNode = createNode(loc, noAccessiblePaths);
    auto newNode = createNode(loc, noAccessiblePaths);
    // Make sure that node is not a nullptr
    if (existingNode == nullptr || newNode == nullptr) {
      std::cout << "FAILURE: node is a nullptr" << std::endl;
    }
    else {
    // Make sure that newNode is the same as existingNode
    if (newNode != existingNode) {
      std::cout << "FAILURE: newNode is not the same as existingNode" << std::endl;
    }
    else {
      std::cout << "SUCCESS: newNode is the same as existingNode" << std::endl;
    }
    }
    break;
  }
  default:
    std::cout<< "Test " << test_id << " of createNode() does not exist" << std::endl;
    // list valid switch cases
    std::cout << "Valid test cases are: " << std::endl;
    std::cout << "1: Node is created successfully" << std::endl;
    std::cout << "2: Node is not a nullptr" << std::endl;
    std::cout << "3: Multiple nodes can be created with different locations" << std::endl;
    std::cout << "4: If a node with the same location already exists, createNode should the existing node" << std::endl;

    return INVALID_TEST_CASE; 

  }
  return EXIT_SUCCESS;
};


void generalDirectionTest(Node &fromNode, Node &toNode, std::string direction, std::vector<std::string> nulldDirections) {
  std::string testDirection = "";
  bool testPassed = true;
  // Use another for loop
  for (auto nullDirection : nulldDirections) {
    testDirection = nullDirection;
    Node *returnedNode = getNeighbour(testDirection, fromNode);
    if (returnedNode != nullptr)
    {
      std::cout << "FAILURE: Returned a node for direction " << testDirection << " when it should have been a nullptr" << std::endl;
      testPassed = false;
    }
  }
  if (testPassed) {
      testDirection = direction;
      // Check if the returned node is node1 when going to the direction above
      Node* returnedNode = getNeighbour(direction, fromNode);
      // make sure the returned node is not a nullptr
      if (returnedNode == nullptr) {
        std::cout << "FAILURE: Returned a nullptr for direction " << testDirection << " when it should have been a node" << std::endl;
      }
      else {
        // make sure the returned node is node1
        if (returnedNode != &toNode) {
          std::cout << "FAILURE: Returned a node for direction " << testDirection << " when it should have been node1" << std::endl;
        }
        else {
          std::cout << "SUCCESS: Returned node1 for direction " << testDirection << std::endl;
        }
      }
  }
}

/**
 * @brief tests the getNeighbour function
 * 
 * @param test_id  - the id of the test case
 * @return int  - EXIT_SUCCESS if the test_id is valid, INVALID_TEST_CASE if the test_id is invalid
 */
int testGetNeighbour(int test_id) {
    std:: cout << "TEST " << test_id << ": getNeighbour() " << std::endl;
    std::string testDirection;

    std::pair<int,int> loc{2, 2};


    // create map of inAccessible paths using the constant directions LEFT, RIGHT, ABOVE, BELOW
    std::map<std::string, Node*> inAccessiblePaths = {{"left", nullptr}, {"above", nullptr}, {"right", nullptr}, {"below", nullptr}};
    
    Node *node = createNode(loc, inAccessiblePaths);

    switch (test_id)
    {
    case 1:
    {
      std::string description = "Should return nullptr with all directions when the node is not connected to any other nodes, aka the node is a dead end";
      printDescription(description);
      // Create a vector of directions
      std::vector<std::string> directions = {"left", "above", "right", "below"};
      // Loop through the directions and check if the returned node is nullptr
      bool testPassed = true;
      for (auto direction : directions) {
        testDirection = direction;
        Node *returnedNode = getNeighbour(testDirection, *node);
        if (returnedNode != nullptr)
        {
          std::cout << "FAILURE: Returned a node for direction " << testDirection << " when it should have been a nullptr" << std::endl;
          testPassed = false;
        }
      }
      if(testPassed) {
        std::cout << "SUCCESS: All directions returned nullptr" << std::endl;
      }
      break;
    }
    case 2:
    {
      testDirection = ABOVE;
      std::string description = "Check if getNeighbour returns a node when going to the direction above, but returns a nullptr when going to the other directions";
      printDescription(description);
      std::pair<int,int> loc1{2, 1};
      std::map<std::string, Node*> pathAbove = {{"left", nullptr}, {"above", node}, {"right", nullptr}, {"below", nullptr}};
      Node *node1 = createNode(loc1, pathAbove);

      // Create a vector of directions
      std::vector<std::string> directions = {"left", "right", "below"};
      // Loop through the directions and check if the returned node is nullptr
      generalDirectionTest(*node1, *node, testDirection, directions);
      break;

    }
    case 3:
    {
      testDirection = BELOW;
      std::string description = "Check if getNeighbour returns a node when going to the direction below, but returns a nullptr when going to the other directions";
      printDescription(description);
      std::pair<int,int> loc1{2, 3};
      std::map<std::string, Node*> pathBelow = {{"left", nullptr}, {"above", nullptr}, {"right", nullptr}, {"below", node}};
      Node *node1 = createNode(loc1, pathBelow);
      
      // Create a vector of directions
      std::vector<std::string> directions = {"left", "right", "above"};
      // Loop through the directions and check if the returned node is nullptr
      generalDirectionTest(*node1, *node, testDirection, directions);
      break;
    }
    case 4:
    {
      testDirection = RIGHT;
      std::string description = "Check if getNeighbour returns a node when going to the direction right, but returns a nullptr when going to the other directions";
      printDescription(description);
      std::pair<int,int> loc1{1, 2};
      std::map<std::string, Node*> pathRight = {{"left", nullptr}, {"above", nullptr}, {"right", node}, {"below", nullptr}};
      Node *node1 = createNode(loc1, pathRight);

      // Create a vector of directions
      std::vector<std::string> directions = {"left", "above", "below"};
      generalDirectionTest(*node1, *node, testDirection, directions);
      break;
    }
    case 5:
    {
      testDirection = LEFT;
      std::string description = "Check if getNeighbour returns a node when going to the direction left, but returns a nullptr when going to the other directions";
      printDescription(description);
      std::pair<int,int> loc1{3, 2};
      std::map<std::string, Node*> pathLeft = {{"left", node}, {"above", nullptr}, {"right", nullptr}, {"below", nullptr}};
      Node *node1 = createNode(loc1, pathLeft);

      // Create a vector of directions
      std::vector<std::string> directions = {"right", "above", "below"};
      generalDirectionTest(*node1, *node, testDirection, directions);
      break;
    }
    default:
      std::cout<< "Test " << test_id << " of getNeighbour() does not exist" << std::endl;
      // list valid switch cases
      std::cout<< "Valid test cases are: 1, 2, 3, 4, 5" << std::endl;
      return INVALID_TEST_CASE;
    }
    return EXIT_SUCCESS;
}

int testConnectNodes(int test_id) {
  std:: cout << "TEST " << test_id << ": connectNodes() " << std::endl;
  std::map<std::string, Node*> startingPaths = {{"left", nullptr}, {"above", nullptr}, {"right", nullptr}, {"below", nullptr}};

  std::pair<int,int> loc1 = {1, 1};
  std::pair<int,int> loc2 = {1, 2};
  std::pair<int,int> loc3 = {2, 1};
  
  // This test needs createNode to work
  std::cout << "Creating nodes with createNode (needs to work)" << std::endl;
  auto startNode = createNode(loc1, startingPaths);
  auto aboveStartNode = createNode(loc2, startingPaths);
  auto rightNode = createNode(loc3, startingPaths);
  // Check that rightNode or aboveStartNode is not a nullptr
  if(startNode == nullptr || aboveStartNode == nullptr || rightNode == nullptr) {
    std::cout << "FAILURE: One of the nodes is a nullptr. Have you implemented createNodes yet?" << std::endl;
    return EXIT_FAILURE;
  }
  
  connectNodes(*startNode, *aboveStartNode);
  connectNodes(*startNode, *rightNode);


  switch (test_id)
  {
  case 1: 
  {
    // Test that all the locations had correct paths
    std::string description = "Test that all the nodes are correctly connected";
    printDescription(description);
    // Check if the nodes are connected
    getNeighbour("above", *startNode) == aboveStartNode ? std::cout << "SUCCESS: startNode is connected to node above: aboveStartNode" << std::endl : std::cout << "FAILURE: startNode is not connected to node above: aboveStartNode" << std::endl;
    getNeighbour("below", *aboveStartNode) == startNode ? std::cout << "SUCCESS: aboveStartnode is connected to node below: startNode." << std::endl : std::cout << "FAILURE: aboveStartNode is not connected to node below: startNode" << std::endl;
    getNeighbour("right", *startNode) == rightNode ? std::cout << "SUCCESS: Node left is connected to node right" << std::endl : std::cout << "FAILURE: Node left is not connected to node right" << std::endl;
    getNeighbour("left", *rightNode) == startNode ? std::cout << "SUCCESS: Node right is connected to node left" << std::endl : std::cout << "FAILURE: Node right is not connected to node left" << std::endl;
    break;
  }
  case 2: {
    // Test that the nodes shouldn't become connected, because they are not neighbours
    std::string description = "Test that the nodes shouldn't become connected, because they are not neighbours";
    printDescription(description);
    
    // Attempt to connect the nodes (Shouldn't work)
    connectNodes(*aboveStartNode, *rightNode);
    
    // Check that the nodes are not connected
    getNeighbour("above", *rightNode) == nullptr ? std::cout << "SUCCESS: Node below is not connected to node right" << std::endl : std::cout << "FAILURE: Node below is connected to node right" << std::endl;
    getNeighbour("right", *aboveStartNode) == nullptr ? std::cout << "SUCCESS: Node above is not connected to node right" << std::endl : std::cout << "FAILURE: Node above is connected to node right" << std::endl;

    break;
  }
  
  default:
    std::cout<< "Test " << test_id << " of connectNodes() does not exist" << std::endl;
    // list valid switch cases
    std::cout << "Valid test cases are: " << std::endl;
    std::cout << "1: Test that all the nodes are correctly connected" << std::endl;
    std::cout << "2: Test that the nodes shouldn't become connected, because they are not neighbours" << std::endl;
    return INVALID_TEST_CASE;
  }
  return EXIT_SUCCESS;
}


/**
 * @brief Creates the maze and returns the start and end node
 * 
 * @param maze - the maze locations 
 * @param shortestPathway - the shortest pathway from start to end
 * @param longestPathway - the longest pathway from start to end
 * @return std::pair<Node*, Node*> 
 */
std::pair<Node*, Node*> createMaze(std::vector<std::pair<int, int>> maze, std::vector<std::pair<int, int>> shortestPathway, std::vector<std::pair<int, int>> longestPathway) {
  std::map<std::string, Node*> startingPaths = {{"left", nullptr}, {"above", nullptr}, {"right", nullptr}, {"below", nullptr}};
  // Create a container for the shortest path from start to end
  std::map<int, Node *> shortestPathNodesToConnect;
  // Create a container for a longer path than the shortest path from start to end
  std::map<int, Node *> longestPathNodesToConnect;

  // Varaible for the start location
  std::pair<int, int> startLocation = shortestPathway[0];
  // Variable for the end location
  std::pair<int, int> endLocation = shortestPathway[shortestPathway.size() - 1];
  // variable for saving starting node
  Node* startNode;
  // variable for saving end node
  Node* endNode;
  for (std::pair<int,int> location : maze) {

    Node* node = createNode(location, startingPaths);
    // If the location is the start location, save the node as the start node
    if (location == startLocation) {
      startNode = node;
    }

    // If the location is the end location, save the node as the end node
    else if (location == endLocation) {
      endNode = node;
    }
    // Check to see if the location is at shortestPathway

    auto it = std::find(shortestPathway.begin(), shortestPathway.end(), location);
      if (it != shortestPathway.end()) {
      // Find its index in the paths vector
      int index = std::distance(shortestPathway.begin(), it);
      // Add the node to the shortestPathNodesToConnect map at the index
      shortestPathNodesToConnect[index] = node;
    }
    // Check to see if the location is at longestPathway
    auto it2 = std::find(longestPathway.begin(), longestPathway.end(), location);
    if (it2 != longestPathway.end()) {
      // Find its index in the paths vector
      int index = std::distance(longestPathway.begin(), it2);
      // Add the node to the longestPathNodesToConnect map at the index
      longestPathNodesToConnect[index] = node;
    }

  }



  // Connect the nodes in the shortest path
  for (int i = 1; i < shortestPathway.size(); ++i) {
    connectNodes(*shortestPathNodesToConnect[i-1], *shortestPathNodesToConnect[i]);
  }
  // Connect the nodes in the longest path
  for (int i = 1; i < longestPathway.size(); ++i) {
    connectNodes(*longestPathNodesToConnect[i-1], *longestPathNodesToConnect[i]);
  }
  return std::make_pair(startNode, endNode);

}


/**
 * @brief Test the function findShortestPath
 *  
 * @param test_id the test case to run
 * @return int - EXIT_SUCCESS if the test is valid, INVALID_TEST_CASE if the test_id is invalid
 */
int testFindShortestPath(int test_id) {
  std:: cout << "TEST " << test_id << ": findShortestPath() " << std::endl;

  
  // This is the maze grid that we will be using for the tests
  std::vector<std::pair<int,int>> locations = {
    {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5},
    {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4},
    {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3},
    {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2},
    {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, 
  };
  // create an L shaped path from {1, 1} to {5, 5} in the maze grid
  std::vector<std::pair<int,int>> lPath = {
    {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}
  };
  // create a snake path from {1, 1} to {5, 5} in the maze grid
  std::vector<std::pair<int,int>> snakePath = {
    {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {2,3}, {3, 3}, {4, 3}, {5, 3}, {5, 4}, {5, 5}
  };


  // Create a pi shaped path from {1, 1} to {5, 1} in the maze grid
  std::vector<std::pair<int,int>> piPath = {
    {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {5, 4}, {5, 3}, {5, 2}, {5, 1}
  };
  std::vector<std::pair<int,int>> straightPath = {
    {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}
  };


  std::vector<std::pair<int, int>> shortestPath;

  switch (test_id)
  {
  case 1: 
  {
    // Test that the shortest path is found and that the length of the vector is the shortest path
    std::string description = "Test that the shortest path is found and that the size of the vector is the shortest path: " + std::to_string(lPath.size());
    printDescription(description);
    // use createMaze to create the nodes and connect them
    std::pair<Node *, Node *> nodes =   createMaze(locations, lPath, snakePath);
    Node *startNode = nodes.first;
    Node* endNode = nodes.second;
    // Find the shortest path
    bool found = findShortestPath(*startNode, *endNode, shortestPath);
    // Check that the shortest path was found
    found ? std::cout << "SUCCESS: Shortest path was found" << std::endl : std::cout << "FAILURE: Shortest path was not found" << std::endl;
    // Check that the shortest path is the correct length
    if(found) {
      shortestPath.size() == lPath.size() ? std::cout << "SUCCESS: Shortest path is the correct length" << std::endl : std::cout << "FAILURE: Shortest path is not the correct length" << std::endl;
    }
    break;
  }
  case 2: {
    // Test that returned vector has all the locations in the shortest path from start to end
    std::string description = "Test that returned vector has all the locations in the shortest path from start to end";
    printDescription(description);
    // use createMaze to create the nodes and connect them
    std::pair<Node *, Node *> nodes =   createMaze(locations, straightPath, piPath);

    Node *startNode = nodes.first;
    Node* endNode = nodes.second;
    // Find the shortest path
    bool found = findShortestPath(*startNode, *endNode, shortestPath);
    // Check that the shortest path is found and it's equal to the straightPath vector
    if(found) {

    std::cout << "EXPECTED vector: " << std::endl;
    printVectorOfPairs(straightPath);
    std::cout << "RETURNED vector " << std::endl;
    printVectorOfPairs(shortestPath);
    if(shortestPath == straightPath) {
      std::cout << "SUCCESS: The shortest path is found and it's equal to the lPath vector" << std::endl;
    } else {
      std::cout << "FAILURE: The shortest path is not found or it's not equal to the lPath vector" << std::endl;
    }
    }
    else {
      std::cout << "FAILURE: The shortest path is not found" << std::endl;
    }

    break;
  }
  case 3: {
    // Test that the shortest path is found to be 0 when the start and end node are the same
    std::string description = "Test that the shortest path is found to be 0 when the start and end node are the same";
    printDescription(description);

    // use createMaze to create the nodes and connect them
    std::pair<Node *, Node *> nodes =   createMaze(locations, lPath, snakePath);
  
    Node *startNode = nodes.first;
    Node* endNode = nodes.first;
    // Find the shortest path
    bool found = findShortestPath(*startNode, *endNode, shortestPath);
    // Check that the shortest path is found 
    if(found) {

    // Check that it's equal to the size of 0
    shortestPath.size() == 0 ? std::cout << "SUCCESS: The shortest path is found and it's size is equal to 0" << std::endl : std::cout << "FAILURE: Its size is not equal to 0" << std::endl;
    break; 
    }
    else {
      std::cout << "FAILURE: The shortest path is not found" << std::endl;
    }
    break; 
  }
  case 4: {
    // Test that the shortest path is found to be 0 when the start and end node are the same
    std::string description = "Test that the shortest path is not found when the start and end node are not connected";
    printDescription(description);

    // use createMaze to create the nodes and connect them
    std::pair<Node *, Node *> nodes = createMaze(locations, lPath, snakePath);
    Node *startNode = nodes.first;
    // create paths for a node

    // Create a new node that is not connected to the graph
    std::map<std::string, Node*> startingPaths = {{"left", nullptr}, {"above", nullptr}, {"right", nullptr}, {"below", nullptr}};
    Node* endNode = createNode({6,6}, startingPaths);
    // Find the shortest path
    bool found = findShortestPath(*startNode, *endNode, shortestPath);
    // Check that the shortest path is not found
    found ? std::cout << "FAILURE: The shortest path is found" << std::endl : std::cout << "SUCCESS: The shortest path is not found" << std::endl;
    break; 
  }
  default: {
    std::cout<< "Test " << test_id << " of findShortestPath() does not exist" << std::endl;
    // list valid switch cases
    std::cout << "Valid test cases are: " << std::endl;
    std::cout << "1: Test that the shortest path is found and that the length of the vector is the shortest path" << std::endl;
    std::cout << "2: Test that returned vector has all the nodes in the shortest path, in the correct order (from start to end)" << std::endl;
    std::cout << "3: Test that the shortest path is found to be 0 when the start and end node are the same" << std::endl;
    std::cout << "4: Test that the shortest path is not found when the start and end node are not connected" << std::endl;

    return INVALID_TEST_CASE;
  }
  }
  return EXIT_SUCCESS;
}


int test(int func_id, int test_id)
{

  switch (func_id)
  {
  case 1:
  {
    return testCreateNode(test_id);
  }
  case 2:
  {
    return testGetNeighbour(test_id);
  }
  case 3:
  {
    return testConnectNodes(test_id);
  }
  case 4:
  {
    return testFindShortestPath(test_id);
  }
  
  default:
    std::cout << "ERROR: Unknown function to test: " << func_id << std::endl;
    std::cout << "       Valid values are:" << std::endl;
    std::cout << "       1: createNode" << std::endl;
    std::cout << "       2: getNeighbour" << std::endl;
    std::cout << "       3: connectNodes" << std::endl;
    std::cout << "       4: findShortestPath" << std::endl;
  
    usage();
    return 3;
  }
  return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
  // function number must be given as the first command line argument, test number as second.
  if (argc < 3) {
    std::cout << "Must be given atleast 2 arguments to function: func_id and test_id" << std::endl;
    usage();
    return EXIT_FAILURE;
  }

  char *p;
  long l_func = strtol(argv[1], &p, 10);
  int func_id = (int)l_func;

  long l_test = strtol(argv[2], &p, 10);
  int test_id = (int)l_test;

  return test(func_id, test_id);
}
