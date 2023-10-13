#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <string>

enum class Instruction               // OpCode Mappings
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

constexpr int32_t ADDI_OPCODE()
{
    return static_cast<int32_t>(Instruction::ADDI);
}
constexpr int32_t B_OPCODE()
{
    return static_cast<int32_t>(Instruction::B);
}
constexpr int32_t BEQZ_OPCODE()
{
    return static_cast<int32_t>(Instruction::BEQZ);
}
constexpr int32_t BGE_OPCODE()
{
    return static_cast<int32_t>(Instruction::BGE);
}
constexpr int32_t BNE_OPCODE()
{
    return static_cast<int32_t>(Instruction::BNE);
}
constexpr int32_t LA_OPCODE()
{
    return static_cast<int32_t>(Instruction::LA);
}
constexpr int32_t LB_OPCODE()
{
    return static_cast<int32_t>(Instruction::LB);
}
constexpr int32_t LI_OPCODE()
{
    return static_cast<int32_t>(Instruction::LI);
}
constexpr int32_t SUBI_OPCODE()
{
    return static_cast<int32_t>(Instruction::SUBI);
}
constexpr int32_t SYSCALL_OPCODE()
{
    return static_cast<int32_t>(Instruction::SYSCALL);
}
#endif
