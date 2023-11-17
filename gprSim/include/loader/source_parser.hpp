#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include <string>
#include "memory.hpp"

struct BitStream
{
    int32_t size;
    int32_t stream;
};

namespace Parser
{
    int32_t parseInstruction(const std::string& next_instruction, Memory& memory, int32_t* LOCCTR);
    bool isBranchInstruction(const std::string& instruction);
    bool isInstruction(const std::string& instruction);
}

#endif
