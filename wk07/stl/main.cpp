//#include <QCoreApplication>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <string>

#include "test.hh"

using namespace std;


void usage()
{
    cout << "Usage: stl <test_function> [<num_of_items>] [<search_value>]" << endl;
    cout << "  test_function: 1|2|3|4|5|6|7|8 (number of the function to test as in stl1.cc, stl2.cc, ...)" << endl;
    cout << "   num_of_items: number of items in the test vector (optional, default=10)" << endl;
    cout << "   search_value: number to search or limit results (the second parameter to the function when needed, default=25)" << endl << endl;
    cout << "Examples:" << endl;
    cout << "  stl 2        (test sortDesc() with a vector of 10 items)" << endl;
    cout << "  stl 4 30     (test findLastEven() with a vector of 30 items)" << endl;
    cout << "  stl 1 100    (test sortAsc() with a vector of 100 items)" << endl;
    cout << "  stl 3 25 13  (test findGivenValue() with a vector of 25 items and search for 13)" << endl;
    cout << "  stl 8 100 47 (test removeLessThan() with a vector of 100 items and remove items less than 47)" << endl;
}

int getRandom(int upper)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(1, upper);
  return dis(gen);
}

std::vector<int> getRandomVector(int size)
{
  std::vector<int> v;

  for (int i = 0; i < size; i++) {
    v.push_back(getRandom(size * 4));
  }

  return v;
}

std::vector<int> getSequence(int size)
{
  std::vector<int> v(size);

  const int rnd = getRandom(size);

  for (int x = 0; x < size; ++x) {
    v[x] = rnd + x;
  }

  return v;
}

std::map<string, int> getMap(const std::vector<int>& v)
{
  std::map<string, int> m;
  for (std::size_t i = 0; i < v.size(); i++) {
    m.insert( std::pair<string, int>("val" + std::to_string(i) , v[i]) );
  }
  return m;
}

void printVector(const std::vector<int>& v)
{
  std::copy(v.begin(), v.end(), std::ostream_iterator<int>(cout, " "));
	cout << endl << endl;
}

void printMap(const std::map<string, int>& m)
{
  for (auto it = m.begin(); it != m.end(); it++) {
    cout << "{" << it->first << " : " << it->second << "} ";
  }
	cout << endl << endl;
}

void test(int func_id, int size, int search_val)
{
  std::vector<int> v = getRandomVector(size);

  switch (func_id)
  {
  case 1:
  {
    cout << "Testing function sortAsc() with the following data:" << endl << endl;
    printVector(v);

    if (EXIT_FAILURE == sortAsc(v)) {
      cout << "Your function failed and returned EXIT_FAILURE" << endl;
    } else {
      cout << "The vector after sortAsc():" << endl << endl;
      printVector(v);
    }
    break;
  }
  case 2:
  {
    cout << "Testing function sortDesc() with the following data:" << endl << endl;
    printVector(v);

    if (EXIT_FAILURE == sortDesc(v)) {
      cout << "Your function failed and returned EXIT_FAILURE" << endl;
    } else {
      cout << "The vector after sortDesc():" << endl << endl;
      printVector(v);
    }
    break;
  }
  case 3:
  {
    cout << "Testing function findGivenValue() with the following data:" << endl << endl;
    printVector(v);

    auto iter = findGivenValue(v, search_val);
    cout << "Searched for " << search_val;

    if (iter != v.end()) {
      if (*iter == search_val) {
        cout << " and found it at index " << std::distance(v.begin(), iter) << "." << endl;
      } else {
        cout << " but the returned iterator refers to " << *iter << " instead." << endl;
      }
    } else {
      cout << " but couldn't find it." << endl;
    }
    break;
  }
  case 4:
  {
    cout << "Testing function findLastEven() with the following data:" << endl << endl;
    printVector(v);

    auto iter = findLastEven(v);
    cout << "Searched for last even value";

    if (iter != v.rend()) {
      cout << " and found " << *iter << " at index " << v.size() - std::distance(v.rbegin(), iter) - 1 << "." << endl;
    } else {
      cout << " but couldn't find any even values." << endl;
    }
    break;
  }
  case 5:
  {
    cout << "Testing function sortMod3() with the following data:" << endl << endl;
    printVector(v);

    if (EXIT_FAILURE == sortMod3(v)) {
      cout << "Your function failed and returned EXIT_FAILURE" << endl;
    } else {
      cout << "The vector after sortMod3():" << endl << endl;
      printVector(v);
    }
    break;
  }
  case 6:
  {
    map<string, int> m = getMap(v);
    cout << "Testing function findAtLeastGiven() with the following data:" << endl << endl;
    printMap(m);

    int found = findAtLeastGiven(m, search_val);
    cout << "Searched for value at least " << search_val;

    if (found != NOT_FOUND) {
      if (found >= search_val) {
        cout << " and found " << found << endl;
      } else {
        cout << " but function returned " << found << " instead." << endl;
      }
    } else {
      cout << " but couldn't find it." << endl;
    }
    break;
  }
  case 7:
  {
    cout << "Testing function findMedian() with the following data:" << endl << endl;
    printVector(v);

    int median = findMedian(v);
    cout << "Searched for median";

    if (median != NOT_FOUND) {
        cout << " and your function returned " << median << endl;
    } else {
      if (v.size() > 0) {
        cout << " but your function erroneously returned NOT_FOUND" << endl;
      } else {
        cout << " but the vector was empty and your function returned correctly NOT_FOUND." << endl;
      }
    }
    break;
  }
  case 8:
  {
    cout << "Testing function removeLessThan() with the following data:" << endl << endl;
    printVector(v);

    cout << "Tried to remove all elements less than " << search_val << endl;
    if (EXIT_FAILURE == removeLessThan(v, search_val)) {
      cout << "Your function failed and returned EXIT_FAILURE" << endl;
    } else {
      cout << "The vector after removeLessThan():" << endl << endl;
      printVector(v);
    }
    break;
  }
  default:
    cout << "ERROR: Unknown function_to_test: " << func_id << endl;
    cout << "       Valid values are: 1,2,3,4,5,6,7,8" << endl;
    usage();
    break;
  }
}

int main(int argc, char *argv[])
{
  // function number must be given as the first command line argument
  if (argc < 2) {
    usage();
    return EXIT_FAILURE;
  }

  char *p;
  long l_func = strtol(argv[1], &p, 10);
  int func_id = (int)l_func;

  // default value for size and search value
  int size = 10;
  int search = 25;

  if (argc > 2) {
    long l_size = strtol(argv[2], &p, 10);
    size = (int)l_size;

    if (argc > 3) {
      long l_search = strtol(argv[3], &p, 10);
      search = (int)l_search;
    }
  }

  if (size < 0) {
    cout << "ERROR: Invalid number of items: " << size << endl;
    cout << "       Number of items should be an integer larger than or equal to 0." << endl;
    usage();
    return EXIT_FAILURE;
  }

  test(func_id, size, search);
  return EXIT_SUCCESS;
}
