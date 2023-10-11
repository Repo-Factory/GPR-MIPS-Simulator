#pragma once
#include <fstream>
#include <string>
#include <functional>

void handleFileError(std::ifstream& stream, char* file);
std::string getWord(std::ifstream& input);
std::string getLine(std::ifstream& input);
std::string getFirstWordOfLine(const std::string& line);
void iterateTokens(const std::string& line, std::function<void(const std::string&)> performAction);
int countWords(const std::string& str);