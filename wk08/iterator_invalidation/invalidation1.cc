#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;


/**
 * @brief creates a vector with ascending numbers from 0 - n-1
 *
 * @param n the size of the vector to be created
 */
std::vector<int> ascendingVector(int n) {
    std::vector<int> v;
    auto beg1 = v.begin();

    for (int i = 0; i < n; ++i) {
        beg1 = v.insert(beg1, i);
        ++beg1;

    }

    return v;
}

