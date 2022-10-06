/**
 * iteration3.cc
 *
 * Print beginning half of a list
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include <iostream>
#include <iterator> // for iterators
#include <list> // for lists
using namespace std;


void printHalf(const list<int>& lst)
{
  /**
  * Use iterators to go through the list and print elements
  */
    int middle = lst.size()/2;
    for(auto iter = lst.begin(); iter != lst.end(); iter++){
        if(distance(lst.begin(), iter) <= middle ){
             cout<< *iter << " " << endl;
        }
    }
}
