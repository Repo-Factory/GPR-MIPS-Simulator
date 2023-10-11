#pragma once
#include "memory.hpp"
#include "parser.hpp"

class Loader
{
    public:
        void loadProgram(Memory& memory, char* assemblyPath);          // API to load assembly into memory
    private:
        void performFirstPass(Memory& memory, std::ifstream& sourceCode);
        void performSecondPass(Memory& memory, std::ifstream& sourceCode);
        int32_t* LOCCTR = nullptr;                                     // Will keep track of where to load instruction to
};
