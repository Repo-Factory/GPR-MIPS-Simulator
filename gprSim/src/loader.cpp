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
#include "parser.hpp"
#include <functional>

#define DATA_SECTION_IDENTIFIER ".data"
#define TEXT_SECTION_IDENTIFIER ".text"
#define LABEL_IDENTIFIER        ':'
#define COMMENT_IDENTIFIER      '#'
#define WHITE_SPACE_IDENTIFIER  ' '
#define FIRST_CHAR(line) line[0]
#define REMOVE_LAST_CHAR(string) string.substr(0, string.size()-1)
#define INCREMENT(ptr) ptr+=1

namespace // Keep private implementation out of Public API with anonymous namespace
{
    /* Reads through file, skips commented lines and loads other words into memory  */
    void iterateFile(std::ifstream& sourceCode, std::function<void(const std::string&)> performActionOnLine)
    {
        while (!sourceCode.eof()) {
            const std::string line = getLine(sourceCode);
            if (line.find_first_not_of(WHITE_SPACE_IDENTIFIER) == std::string::npos) continue; // Skip empty lines
            if (FIRST_CHAR(line) == COMMENT_IDENTIFIER)                              continue; // Skip commented lines
            performActionOnLine(line);
        }
    }
}

namespace // First Pass Line Handling Helpers
{
    void updateMemorySectionIfNecessary(int32_t* LOCCTR, Memory& memory, const std::string& line)
    {
        const std::string firstWord = getFirstWordOfLine(line);
        if      (firstWord == DATA_SECTION_IDENTIFIER)   LOCCTR = memory.userDataPtr;
        else if (firstWord == TEXT_SECTION_IDENTIFIER)   LOCCTR = memory.userTextPtr;
    }

    void addSymbolTableEntryIfNecessary(int32_t* LOCCTR, Memory& memory, const std::string& line)
    {
        const std::string firstWord = getFirstWordOfLine(line);
        if (firstWord.back() == LABEL_IDENTIFIER) {
            memory.symbol_table.insert({REMOVE_LAST_CHAR(firstWord), LOCCTR});
        }
    }

    void incrementLocationCounterIfNecessary(int32_t* LOCCTR, const std::string& line)
    {
        if (Parser::isInstruction(line)) INCREMENT(LOCCTR);
    }
}

/* 
 * Create Symbol Table
 */
void Loader::performFirstPass(Memory& memory, std::ifstream& sourceCode)
{ 
    iterateFile(sourceCode, [&](const std::string& line) {
        updateMemorySectionIfNecessary(this->LOCCTR, memory, line);
        addSymbolTableEntryIfNecessary(this->LOCCTR, memory, line);
        incrementLocationCounterIfNecessary(this->LOCCTR, line);
    });
}

/* 
 * Convert instructions to 32-bit streams and load them into memory
 */
void Loader::performSecondPass(Memory& memory, std::ifstream& sourceCode)
{ 
    iterateFile(sourceCode, [&](const std::string& line) {
        updateMemorySectionIfNecessary(this->LOCCTR, memory, line);
        if (Parser::isInstruction(line)) {
            writeContents(this->LOCCTR++, Parser::parseInstruction(line, memory));
        }
    });
}

/* Simple entry point to load program which will open the program, iterate through it, and close it */
void Loader::loadProgram(Memory& memory, char* assemblyPath)
{
    std::ifstream sourceCode(assemblyPath);
    handleFileError(sourceCode, assemblyPath);
    this->performFirstPass(memory, sourceCode);
    this->performSecondPass(memory, sourceCode);
    #ifdef DEBUG
        debugPrint(memory);
    #endif
    sourceCode.close();
}