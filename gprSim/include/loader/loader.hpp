#ifndef LOADER_H
#define LOADER_H

#include "memory.hpp"

class Loader
{
    public:
        void loadProgram(Memory& memory, const char* assemblyPath);          // API to load assembly into memory
    private:
        void performFirstPass(Memory& memory, std::ifstream& sourceCode);
        void performSecondPass(Memory& memory, std::ifstream& sourceCode);
        int32_t* LOCCTR = nullptr;                                     // Will keep track of where to load instruction to
};

namespace SymbolTable
{
    void addSymbolTableEntryIfNecessary(int32_t* LOCCTR, Memory& memory, const std::string& line);
}

#endif