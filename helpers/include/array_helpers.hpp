#ifndef ARRAY_HELPERS_H
#define ARRAY_HELPERS_H

#include <functional>

int sumArray(const int* array, const int size);
void overlayArrays(int* targetArray, const int* sourceArray, const int size);
void forEachItem(const int* array, const int size, std::function<void(int, int)> action);

#endif
