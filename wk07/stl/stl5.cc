#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;


/**
 * @brief Arrange vector in three subsequent sections:
 *        - those divisible by three (asc order)
 *        - those whose reminder is 1 (asc order)
 *        - those whose reminder is 2 (asc order)
 * @param v a sortable vector
 * @return int EXIT_SUCCESS if everything went OK, EXIT_FAILURE otherwise
 */

bool myfunction(int i, int j){
    if(i%3 == 0 && j%3 ==1){
        return true;
    }
    else if(i%3 == 0 && j%3 == 2){
        return true;
    }
    else if(i%3 == 1 && j%3 == 2){
        return true;
    }


    else if(j%3 == 0 && i%3 == 1){
        return false;
    }
    else if(j%3 == 0 && i%3 == 2){
        return false;
    }
    else if(j%3 == 1 && i%3 == 2){
        return false;
    }

    else{
        return i < j;
    }
}

int sortMod3(std::vector<int>& v)
{
    partial_sort(v.begin(), v.end(), v.end(), myfunction);
    return EXIT_SUCCESS;
}

