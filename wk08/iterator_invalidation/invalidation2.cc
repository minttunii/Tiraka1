#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;


/**
 * @brief Erases every second item from the vector. Example: {1, 2, 3, 4} -> {1, 3}
 *
 * @param vec vector where every second item is erased.
 */
void eraseEverySecond(std::vector<int>& vec) {
    auto beg = vec.begin()+1;
    auto end = vec.end();

    for (auto i = beg; i != end;){
        i = vec.erase(i);
        end = vec.end();

        if(i != end){
            ++i;
        }
    }
}

