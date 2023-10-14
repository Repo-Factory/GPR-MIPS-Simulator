/*
 *  @brief
 *          Assists loader with symbol mapping
 *
 * Every time a label is seen we have to appropriately handle and store the address it points to and allocate the necessary data
 * at that address
 */


#include "loader.hpp"
#include "debug_helpers.hpp"
#include "file_handling_helpers.hpp"
#include "source_parser.hpp"
#include <functional>

#define LABEL_IDENTIFIER        ':'
#define REMOVE_COLON(string) string.substr(0, string.size()-1)

constexpr const char* ASCII_IDENTIFIER       = "\".asciiz\"";
constexpr const char* BYTES_IDENTIFER        = "\".space\"";
constexpr const char* COMMENT_IDENTIFIER     = "#";
constexpr const char* INVALID_OPTION_MESSAGE = "%s is Not a Valid Data Section Identifier";
constexpr const int   JUST_LABEL             = 1;

namespace // Dynamic Allocations for data section entries
{
    void* handleASCIIData(std::istringstream& tokens)
    {
        std::string data;
        std::getline(tokens >> std::ws, data);       // Store the rest of the line (which will be the string it holds) 
        std::string* entry = new std::string();      // We have to dynamically allocate string to be accessed later during program execution
        *entry = data;
        return (void*)entry;
    }

    void* handleBytesData(std::istringstream& tokens)
    {
        std::string word;
        tokens >> word;
        const int size = std::stoi(word);
        char* entry = (char*)malloc(size + 1);  // Allocate memory for 'size' characters and a null-terminator
        char* ptr = entry;
        for (int i = 0; i < size; i++) {        // Set all characters to null
            *ptr++ = '\0';
        }
        return (void*)entry;
    }
}

namespace
{
    // Data entry in assembly looks like ------->   label ".type" data
    void* allocateDataEntry(const std::string& line)
    {
        std::istringstream tokens(line);// Tokenize line
        std::string word;
        tokens >> word;                 // Skip first  which is the label
        tokens >> word;                 // Check the second word for type
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

namespace
{
    std::string trimComments(const std::string& line)
    {
        std::string copy = line;
        size_t pos = copy.find(COMMENT_IDENTIFIER);
        if (pos != std::string::npos) {
            copy=copy.substr(0,pos);
        }
        return copy;
    }

    void handleTextLabel(const std::string& label, int32_t* LOCCTR, Memory& memory)
    {
        memory.symbol_table.insert({REMOVE_COLON(label), (void*)LOCCTR});
    }

    void handleDataLabel(const std::string& label, const std::string& line, Memory& memory)
    {
        memory.symbol_table.insert({REMOVE_COLON(label), allocateDataEntry(line)});
    }
}

/* If a label is found, adds it to the table accordingly based on if its a text or data section label */
void SymbolTable::addSymbolTableEntryIfNecessary(int32_t* LOCCTR, Memory& memory, const std::string& line)
{
    const std::string firstWord = getFirstWordOfLine(trimComments(line));
    if (firstWord.back() == LABEL_IDENTIFIER) {
        if (countWords(trimComments(line)) == JUST_LABEL) {
             handleTextLabel(firstWord, LOCCTR, memory);
        }
        else {
            handleDataLabel(firstWord, trimComments(line), memory);
        }
    }
}

