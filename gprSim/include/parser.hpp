#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include <string>

enum class Instructions
{
    ADDI                            = 0,
    B                               = 1,
    BEQZ                            = 2,
    BGE                             = 3,
    BNE                             = 4,
    LA                              = 5,
    LB                              = 6,
    LI                              = 7,
    SUBI                            = 8,
    SYSCALL                         = 9
};

namespace Parser
{
    int32_t parseInstruction(const std::string& next_instruction);
    bool    isInstruction(const std::string& instruction);
}

#endif
