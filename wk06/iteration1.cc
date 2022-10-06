/**
 * iteration1.cc
 *
 * Print all items of a list
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include <iostream>
#include <iterator> // for iterators
#include <list> // for lists
using namespace std;


void printAllItems(const list<int>& lst)
{
  /**
  * Use iterators to go through the list and print elements
  */


  for(auto iter = lst.begin(); iter != lst.end(); iter++){
      cout<< *iter << " " << endl;
  }
}
