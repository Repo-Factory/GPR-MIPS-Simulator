#pragma once
#include "memory.hpp"

class Loader
{
    public:
        void loadProgram(Memory& memory, char* assemblyPath);          // API to load assembly into memory
    private:
        void performFirstPass(Memory& memory, std::ifstream& sourceCode);
        void performSecondPass(Memory& memory, std::ifstream& sourceCode);
        int32_t* LOCCTR = nullptr;                                     // Will keep track of where to load instruction to
};

namespace SymbolTable
{
    int32_t* addSymbolTableEntryIfNecessary(int32_t* LOCCTR, Memory& memory, const std::string& line);
}