#pragma once
#include <fstream>
#include <string>

void handleFileError(std::ifstream& stream, char* file);
std::string getWord(std::ifstream& input);
std::string getLine(std::ifstream& input);