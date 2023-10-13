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
#define COMMENT_IDENTIFIER_LITERAL      '#'
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
            if (FIRST_CHAR(line) == COMMENT_IDENTIFIER_LITERAL)                              continue; // Skip commented lines
            performActionOnLine(line);
        }
    }
}

void* handleASCIIData(std::istringstream& tokens)
{
    std::string data;
    std::getline(tokens >> std::ws, data);
    std::string* entry = new std::string();      // Allocate a byte for each character of string
    *entry = data;
    return (void*)entry;
}

void* handleBytesData(std::istringstream& tokens)
{
    std::string word;
    tokens >> word;
    return (void*)malloc(std::stoi(word) * sizeof(char));   // Allocate number of bytes indicated
}

namespace
{
    constexpr const char* ASCII_IDENTIFIER   = "\".asciiz\"";
    constexpr const char* BYTES_IDENTIFER    = "\".space\"";
    constexpr const char* COMMENT_IDENTIFIER = "#";
    constexpr const char* INVALID_OPTION_MESSAGE = "%s is Not a Valid Data Section Identifier";

    void* allocateDataEntry(const std::string& line)
    {
        std::istringstream tokens(line);
        std::string word;
        tokens >> word;         // Skip first  which is the label
        tokens >> word;         // Check the second word for type
        if (word==COMMENT_IDENTIFIER) {
            return NULL;
        }
        if (word==ASCII_IDENTIFIER) {
            return handleASCIIData(tokens);
        }
        if (word == BYTES_IDENTIFER) {
            return handleBytesData(tokens);
        }
        printf(INVALID_OPTION_MESSAGE, word.c_str());
        exit(EXIT_FAILURE);
    }
}

namespace // First Pass Line Handling Helpers
{
    constexpr const int JUST_LABEL = 1;
    int32_t* updateMemorySectionIfNecessary(int32_t* LOCCTR, Memory& memory, const std::string& line)
    {
        const std::string firstWord = getFirstWordOfLine(line);
        if      (firstWord == DATA_SECTION_IDENTIFIER)   return memory.userDataPtr;
        else if (firstWord == TEXT_SECTION_IDENTIFIER)   return memory.userTextPtr;
        else    return LOCCTR;
    }

    int32_t* addSymbolTableEntryIfNecessary(int32_t* LOCCTR, Memory& memory, const std::string& line)
    {
        std::string copy = line;
        size_t pos = copy.find("#");
        if (pos != std::string::npos) copy=copy.substr(0,pos);
        const std::string firstWord = getFirstWordOfLine(copy);
        if (firstWord.back() == LABEL_IDENTIFIER) {
            if (countWords(copy) == JUST_LABEL) {
                memory.symbol_table.insert({REMOVE_LAST_CHAR(firstWord), (void*)LOCCTR});
            }
            else {
                void* entry = allocateDataEntry(copy);
                if (entry) {
                    memory.symbol_table.insert({REMOVE_LAST_CHAR(firstWord), entry});
                }
            }
        }
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
        this->LOCCTR = addSymbolTableEntryIfNecessary(this->LOCCTR, memory, line);
        if (Parser::isInstruction(line)) {
            INCREMENT(LOCCTR);
        }
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
            writeContents(this->LOCCTR++, Parser::parseInstruction(line, memory));
        }
    });
}

void debugPrint(Memory& memory)
{
    for (int i = 0; i < 256; ++i) {
        std::cout << "Memory[" << i << "]: "; printBinary(*((int32_t*)&memory + i));
    }
    std::cout << &memory + 5648;
    // std::cout << *(std::string*)memory.symbol_table["is_palin_msg"];
    // std::cout << (char*)memory.symbol_table["is_palin_msg"] - (char*)&memory;
    std::cout << memory.symbol_table;
}

namespace
{
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