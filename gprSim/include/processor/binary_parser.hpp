#ifndef BINARY_PARSER_H
#define BINARY_PARSER_H

#include "stdio.h"
#include "stdlib.h"
#include "instructions.hpp"
#include "cpu.hpp"

namespace BinaryParser
{
    ADDI_Instruction    PARSE_ADDI  (const int32_t instruction, MIPSCPU& cpu);
    B_Instruction       PARSE_B     (const int32_t instruction, MIPSCPU& cpu);
    BEQZ_Instruction    PARSE_BEQZ  (const int32_t instruction, MIPSCPU& cpu);
    BGE_Instruction     PARSE_BGE   (const int32_t instruction, MIPSCPU& cpu);
    BNE_Instruction     PARSE_BNE   (const int32_t instruction, MIPSCPU& cpu);
    LA_Instruction      PARSE_LA    (const int32_t instruction, MIPSCPU& cpu);
    LB_Instruction      PARSE_LB    (const int32_t instruction, MIPSCPU& cpu);
    LI_Instruction      PARSE_LI    (const int32_t instruction, MIPSCPU& cpu);
    SUBI_Instruction    PARSE_SUBI  (const int32_t instruction, MIPSCPU& cpu);
}

#endif