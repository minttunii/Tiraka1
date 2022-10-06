/**
 * iteration4.cc
 *
 * Print all items of a list in a reverse order
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include <iostream>
#include <iterator> // for iterators
#include <list> // for lists
using namespace std;


void printReverse(const list<int>& lst)
{
  /**
  * Use iterators to go through the list and print elements
  */

    for(auto iter = lst.rbegin(); iter != lst.rend(); iter++){
        cout<< *iter << " ";
    }
    cout<<endl;
}
