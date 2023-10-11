#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include <string>
#include "memory.hpp"

struct BitStream
{
    int32_t stream;
    int32_t size;
};

namespace Parser
{
    int32_t parseInstruction(const std::string& next_instruction, const Memory& memory);
    bool    isInstruction(const std::string& instruction);
}

#endif
