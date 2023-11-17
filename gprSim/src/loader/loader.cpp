/*
 *  @brief
 *          Loads program into memory
 *
 * Goes through assembly file, converts each symbol to a bit stream and places them in appropriate parts of memory
 * Loader class has current LOCCTR pointer to point to the correct place in memory to load next instruction
 */

#include "loader.hpp"
#include "debug_helpers.hpp"
#include "file_handling_helpers.hpp"
#include "source_parser.hpp"
#include <functional>
#include <iostream>

#define COMMENT_IDENTIFIER_LITERAL      '#'
#define WHITE_SPACE_IDENTIFIER          ' '
#define DATA_SECTION_IDENTIFIER         ".data"
#define TEXT_SECTION_IDENTIFIER         ".text"
#define FIRST_CHAR(line)                line[0]
#define INCREMENT(ptr)                  ptr+=1

void debugPrint(Memory& memory)
{
    std::cout << "_______________ INSTRUCTIONS ______________" << std::endl;
    for (int i = 0; i < 32; ++i) {
        std::cout << "Memory[" << i << "]: "; printBinary(*((int32_t*)&memory + i));
    }  std::cout << "_______________ INSTRUCTIONS ______________"  << std::endl;
    std::cout << std::endl << "________________ SYMBOL TABLE  _____________" << std::endl;
    std::cout << memory.symbol_table;
    std::cout << "________________ SYMBOL TABLE  _____________" << std::endl;
    std::cout << std::endl;
}


/* Reads through file, skips commented or label lines and loads other words into memory  */
void iterateFile(std::ifstream& sourceCode, std::function<void(const std::string&)> performActionOnLine)
{
    while (!sourceCode.eof()) {
        const std::string line = getLine(sourceCode);
        if (line.find_first_not_of(WHITE_SPACE_IDENTIFIER) == std::string::npos) continue; // Skip empty lines
        if (FIRST_CHAR(line) == COMMENT_IDENTIFIER_LITERAL)                      continue; // Skip commented lines
        performActionOnLine(line);
    }
}

// Upon seeing .txt or .data we need to move our LOCCTR to the appropriate location
int32_t* updateMemorySectionIfNecessary(int32_t* LOCCTR, Memory& memory, const std::string& line)
{
    const std::string firstWord = getFirstWordOfLine(line);
    if (firstWord == DATA_SECTION_IDENTIFIER) {
        return memory.userDataPtr;
    }
    else if (firstWord == TEXT_SECTION_IDENTIFIER) {  
        return memory.userTextPtr;
    }
    else {    
        return LOCCTR;
    }
}

/* 
 * Create Symbol Table
 */
void Loader::performFirstPass(Memory& memory, std::ifstream& sourceCode)
{ 
    iterateFile(sourceCode, [&](const std::string& line) {
        this->LOCCTR = updateMemorySectionIfNecessary(this->LOCCTR, memory, line);
        SymbolTable::addSymbolTableEntryIfNecessary(this->LOCCTR, memory, line);
        if (Parser::isInstruction(line)) { this->LOCCTR+=2; }
    });
}

/* 
 * Convert instructions to 32-bit streams and load them into memory
 */
void Loader::performSecondPass(Memory& memory, std::ifstream& sourceCode)
{ 
    iterateFile(sourceCode, [&](const std::string& line) {
        this->LOCCTR = updateMemorySectionIfNecessary(this->LOCCTR, memory, line);
        if (Parser::isInstruction(line)) {
            writeContents(this->LOCCTR++, Parser::parseInstruction(line, memory, this->LOCCTR)); 
            writeContents(this->LOCCTR++, 0);
        }   // Place 32 bit instruction into text section 
    });
}

// Wrapper for file handling
void handlePass(const char* assemblyPath, std::function<void(std::ifstream&)> performPass)
{
    std::ifstream sourceCode = std::ifstream(assemblyPath);
    handleFileError(sourceCode, assemblyPath);
    performPass(sourceCode);
    sourceCode.close();
}

/* Simple entry point to load program from source code to memory. Uses two pass implementation*/
void Loader::loadProgram(Memory& memory, const char* assemblyPath)
{
    handlePass(assemblyPath, [&](std::ifstream& sourceCode){this->performFirstPass(memory, sourceCode);});
    handlePass(assemblyPath, [&](std::ifstream& sourceCode){this->performSecondPass(memory, sourceCode);});
    #ifdef DEBUG
        debugPrint(memory);
    #endif
}