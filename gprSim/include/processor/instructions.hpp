#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <string>

enum class Identifier               // OpCode Mappings
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

namespace OpcodeTable
{
    bool searchTable(const std::string& token);
    int32_t getOpcode(const std::string& token);
}

namespace RegisterTable
{
    bool searchTable(const std::string& token);
    int32_t getRegister(const std::string& token);
}

#endif
