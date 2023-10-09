/* Simple file handling functions that are mostly self explanatory. I didn't want this logic polluting the sim code. */

#include <iostream>
#include <fstream>
#include <string>

void handleFileError(std::ifstream& stream, char* file)
{   
    if (!stream) {
        std::cerr << "Failed to open file: " << file << std::endl;
        exit(EXIT_FAILURE);
    }
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