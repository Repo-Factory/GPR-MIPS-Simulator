/* Simple file handling functions that are mostly self explanatory. I didn't want this logic polluting the sim code. */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <functional>

void handleFileError(std::ifstream& stream, char* file)
{   
    if (!stream) {
        std::cerr << "Failed to open file: " << file << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::string getFirstWordOfLine(const std::string& line)
{
    std::istringstream iss(line);
    std::string firstWord;
    iss >> firstWord;
    return firstWord;
}

std::string getWord(std::ifstream& input) {
    std::string word;
    input >> word;
    return word;
}

std::string getLine(std::ifstream& input) {
    std::string line;
    std::getline(input, line);  
    return line;
}

void iterateTokens(const std::string& line, std::function<void(const std::string&)> performAction)
{
    const std::string COMMENT_IDENTIFIER = "#";
    std::istringstream tokens(line);
    std::string  token;
    while (tokens >> token)
    {
        if (token == COMMENT_IDENTIFIER) break;
        performAction(token);
    }
}

int countWords(const std::string& str) {
    std::istringstream iss(str);
    int wordCount = 0;
    std::string word;
    while (iss >> word) {
        wordCount++;
    }
    return wordCount;
}