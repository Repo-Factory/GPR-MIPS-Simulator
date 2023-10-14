/* 
 * @brief Simple array functions
 */

#include "array_helpers.hpp"

int sumArray(const int* array, const int size)
{
    int sum = 0;
    for (int i=0; i<size; i++) {
        sum += *(array + i);
    }
    return sum;
}          

// Add one array on top of another
void overlayArrays(int* targetArray, const int* sourceArray, const int size)
{
    for (int i=0; i<size; i++) {
        *(targetArray + i) += *(sourceArray + i);
    }
}

void forEachItem(const int* array, const int size, std::function<void(int, int)> action)
{
    for (int i=0; i<size; i++) {
        action(*(array+i), i);
    }
}
