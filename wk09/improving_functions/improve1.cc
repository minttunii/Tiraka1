#include <iterator>
#include <numeric>
#include <vector>
#include <algorithm>


/**
 * @brief creates a vector of integers with ascending numbers from 0 - n-1
 *
 * @param n the size of the vector to be created
 * @return std::vector<int>
 */


std::vector<int> ascendingVector(int n){
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 0);

    return v;
}
