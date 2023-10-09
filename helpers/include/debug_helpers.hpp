#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include "memory.hpp"

std::ostream& operator<<(std::ostream& stream, std::map<int32_t, int32_t*>& table);
std::string int32ToAscii(int32_t intValue);
void debugPrint(Memory& memory);