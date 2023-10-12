#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>

std::ostream& operator<<(std::ostream& stream, std::map<std::string, int32_t*>& table);
std::string int32ToAscii(int32_t intValue);
