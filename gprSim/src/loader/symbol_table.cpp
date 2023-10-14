#include "loader.hpp"
#include "debug_helpers.hpp"
#include "file_handling_helpers.hpp"
#include "source_parser.hpp"
#include <functional>

#define LABEL_IDENTIFIER        ':'
#define REMOVE_LAST_CHAR(string) string.substr(0, string.size()-1)

constexpr const char* ASCII_IDENTIFIER       = "\".asciiz\"";
constexpr const char* BYTES_IDENTIFER        = "\".space\"";
constexpr const char* COMMENT_IDENTIFIER     = "#";
constexpr const char* INVALID_OPTION_MESSAGE = "%s is Not a Valid Data Section Identifier";
constexpr const int   JUST_LABEL             = 1;

namespace
{
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
    void* allocateDataEntry(const std::string& line)
    {
        std::istringstream tokens(line);
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
        memory.symbol_table.insert({REMOVE_LAST_CHAR(label), (void*)LOCCTR});
    }

    void handleDataLabel(const std::string& label, const std::string& line, Memory& memory)
    {
        void* entry = allocateDataEntry(line);
        if (entry) memory.symbol_table.insert({REMOVE_LAST_CHAR(label), entry});
    }
}

int32_t* SymbolTable::addSymbolTableEntryIfNecessary(int32_t* LOCCTR, Memory& memory, const std::string& line)
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
    return LOCCTR;
}

