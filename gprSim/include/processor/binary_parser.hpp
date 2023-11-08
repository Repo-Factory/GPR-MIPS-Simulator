#ifndef BINARY_PARSER_H
#define BINARY_PARSER_H

#include "stdio.h"
#include "stdlib.h"
#include "instructions.hpp"

struct MIPSCPU;

namespace BinaryParser
{
    int32_t             extractOpcode(const int32_t instruction);
    ADDI_Instruction    PARSE_ADDI   (const int32_t instruction, MIPSCPU& cpu);
    B_Instruction       PARSE_B      (const int32_t instruction, MIPSCPU& cpu);
    BEQZ_Instruction    PARSE_BEQZ   (const int32_t instruction, MIPSCPU& cpu);
    BGE_Instruction     PARSE_BGE    (const int32_t instruction, MIPSCPU& cpu);
    BNE_Instruction     PARSE_BNE    (const int32_t instruction, MIPSCPU& cpu);
    LA_Instruction      PARSE_LA     (const int32_t instruction, MIPSCPU& cpu);
    LB_Instruction      PARSE_LB     (const int32_t instruction, MIPSCPU& cpu);
    LI_Instruction      PARSE_LI     (const int32_t instruction, MIPSCPU& cpu);
    SUBI_Instruction    PARSE_SUBI   (const int32_t instruction, MIPSCPU& cpu);
}

struct InstructionType
{
    Instruction type;
    union 
    {
        ADDI_Instruction ADDI_instruction;
        B_Instruction B_instruction;
        BEQZ_Instruction BEQZ_instruction;
        BGE_Instruction BGE_instruction;
        BNE_Instruction BNE_instruction;
        LA_Instruction LA_instruction;
        LB_Instruction LB_instruction;
        LI_Instruction LI_instruction;
        SUBI_Instruction SUBI_instruction;
    };
};

#endif