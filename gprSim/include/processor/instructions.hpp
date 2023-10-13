#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <string>

/* 
 *          Instruction Set 
 *      ----------------------
 *      ADDI Rdest, Rsrc1, Imm
 *      B label
 *      BEQZ Rsrc1, label
 *      BGE Rsrc1, Rsrc2, label
 *      BNE Rsrc1, Rsrc2, label
 *      LA Rdest, label
 *      LB Rdest, offset(Rsrc1)
 *      LI Rdest, Imm
 *      SUBI Rdest, Rsrc1, Imm
 *      SYSCALL
 */

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

struct ADDI_Instruction
{
    int32_t* Rdest;
    int32_t* Rsrc1;
    int32_t Imm;
};

struct B_Instruction
{
    int32_t label;
};

struct BEQZ_Instruction
{
    int32_t* Rsrc1;
    int32_t label;
};

struct BGE_Instruction
{
    int32_t* Rsrc1;
    int32_t* Rsrc2;
    int32_t label;
};

struct BNE_Instruction
{
    int32_t* Rsrc1;
    int32_t* Rsrc2;
    int32_t label;
};

struct LA_Instruction
{
    int32_t* Rdest;
    int32_t label;
};

struct LB_Instruction
{
    int32_t* Rdest;
    int32_t* Rsrc1;
    int32_t offset;
};

struct LI_Instruction
{
    int32_t* Rdest;
    int32_t Imm;
};
struct SUBI_Instruction
{
    int32_t* Rdest;
    int32_t* Rsrc1;
    int32_t Imm;
};

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