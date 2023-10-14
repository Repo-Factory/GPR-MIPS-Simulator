#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <functional>

std::ostream& operator<<(std::ostream& stream, std::map<std::string, void*>& table);
std::string int32ToAscii(int32_t intValue);
void printBinary(int32_t num);
int sumArray(const int* array, const int size);
void overlayArrays(int* targetArray, const int* sourceArray, const int size);
void forEachItem(const int* array, const int size, std::function<void(int, int)> action);