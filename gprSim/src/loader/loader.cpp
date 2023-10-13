/*
 *  @brief
 *          Loads program into memory
 *
 * Goes through assembly file, converts each symbol to a bit stream and places them in appropriate parts of memory
 * Loader class has current address pointer to point to the correct place in memory to load next instruction
 */

#include "loader.hpp"
#include "debug_helpers.hpp"
#include "file_handling_helpers.hpp"
#include "parser.hpp"
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
    for (int i = 0; i < 256; ++i) {
        std::cout << "Memory[" << i << "]: "; printBinary(*((int32_t*)&memory + i));
    }
    int32_t* ptr = (int32_t*)&memory+5648;
    std::cout << *(std::string*)ptr << std::endl;
    std::cout << *(std::string*)memory.symbol_table["is_palin_msg"] << std::endl;
    std::cout << memory.symbol_table;
}


namespace // Keep private implementation out of Public API with anonymous namespace
{
    /* Reads through file, skips commented lines and loads other words into memory  */
    void iterateFile(std::ifstream& sourceCode, std::function<void(const std::string&)> performActionOnLine)
    {
        while (!sourceCode.eof()) {
            const std::string line = getLine(sourceCode);
            if (line.find_first_not_of(WHITE_SPACE_IDENTIFIER) == std::string::npos) continue; // Skip empty lines
            if (FIRST_CHAR(line) == COMMENT_IDENTIFIER_LITERAL)                      continue; // Skip commented lines
            performActionOnLine(line);
        }
    }
}

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
        this->LOCCTR = FirstPass::addSymbolTableEntryIfNecessary(this->LOCCTR, memory, line);
        if (Parser::isInstruction(line)) { INCREMENT(LOCCTR); }
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
        }
    });
}


void handleFirstPass(Loader* loader, Memory& memory, char* assemblyPath)
{
    std::ifstream sourceCode = std::ifstream(assemblyPath);
    handleFileError(sourceCode, assemblyPath);
    loader->performFirstPass(memory, sourceCode);
    sourceCode.close();
}

void handleSecondPass(Loader* loader, Memory& memory, char* assemblyPath)
{
    std::ifstream sourceCode = std::ifstream(assemblyPath);
    handleFileError(sourceCode, assemblyPath);
    loader->performSecondPass(memory, sourceCode);
    sourceCode.close();
}


/* Simple entry point to load program which will open the program, iterate through it, and close it */
void Loader::loadProgram(Memory& memory, char* assemblyPath)
{
    handleFirstPass(this, memory, assemblyPath);
    handleSecondPass(this, memory, assemblyPath);
    #ifdef DEBUG
        debugPrint(memory);
    #endif
}