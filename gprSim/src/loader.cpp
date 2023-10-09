/*
 *  @brief
 *          Loads program into memory
 *
 * Goes through assembly file, converts each symbol to a bit stream and places them in appropriate parts of memory
 * Loader class has current address pointer to point to the correct place in memory to load next instruction
 */

#include "loader.hpp"
#include "debug_helpers.hpp"
#include "bit_operations.hpp"
#include "file_handling_helpers.hpp"
#include <regex>

#define LABEL_REGEX_PATTERN "([A-Za-z][A-Za-z]*:)"
#define INT_REGEX_PATTERN "[0-9]*"
#define DATA_SECTION_IDENTIFIER ".data"
#define TEXT_SECTION_IDENTIFIER ".text"
#define COMMENT_IDENTIFIER '#'

namespace // Keep private implementation out of Public API with anonymous namespace
{
    /* Reads through file, skips commented lines and loads other words into memory  */
    void iterateFile(Loader* loader, Memory& memory, std::ifstream& sourceCode)
    {
        while (!sourceCode.eof()) {
            if (sourceCode.peek() == COMMENT_IDENTIFIER) 
                getLine(sourceCode); // skip line
            else loader->loadToMemory(memory, getWord(sourceCode));
        }
    }
}

/* 
 * Handles the current word. If it corresponds with a section, we change where the loader points to in memory
 * We use regex to identify labels, which will prompt us to store the label in our symbol table
 * Encountering an integer will cause us to load that value into the current memory address
 * Otherwise, we have an instruction to convert to a bit stream and place into the current memory address
 */
void Loader::loadToMemory(Memory& memory, const std::string& word)
{ 
    if (word == DATA_SECTION_IDENTIFIER)
        this->currentAddressPtr = memory.userDataPtr;

    else if (word == TEXT_SECTION_IDENTIFIER) 
        this->currentAddressPtr = memory.userTextPtr;

    else if (std::regex_match(word, std::regex(LABEL_REGEX_PATTERN)))
        memory.symbol_table.insert({labelToBitStream(word), this->currentAddressPtr});

    else if (std::regex_match(word, std::regex(INT_REGEX_PATTERN)))
        writeContents(this->currentAddressPtr++, std::stoi(word));

    else writeContents(this->currentAddressPtr++, stringToBitStream(word));
}

/* Simple entry point to load program which will open the program, iterate through it, and close it */
void Loader::loadProgram(Memory& memory, char* assemblyPath)
{
    std::ifstream sourceCode(assemblyPath);
    handleFileError(sourceCode, assemblyPath);
    iterateFile(this, memory, sourceCode);
    #ifdef DEBUG
        debugPrint(memory);
    #endif
    sourceCode.close();
}  
 