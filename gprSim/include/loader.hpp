#pragma once
#include "memory.hpp"

class Loader
{
    public:
        void loadProgram(Memory& memory, char* assemblyPath);          // API to load assembly into memory
        void loadToMemory(Memory& memory, const std::string& word);    // Helper to load word into memory
    private:
        int32_t* currentAddressPtr = nullptr;                          // Will keep track of where to load instruction to
};
