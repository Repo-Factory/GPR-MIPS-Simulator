#ifndef BINARY_PARSER_H
#define BINARY_PARSER_H

#include "stdio.h"
#include "stdlib.h"
#include "instructions.hpp"

struct MIPSCPU;

namespace BinaryParser
{
    int32_t        extractOpcode(const int32_t instruction);
    ADDI_Result    PARSE_ADDI   (const int32_t instruction, MIPSCPU& cpu);
    B_Result       PARSE_B      (const int32_t instruction, MIPSCPU& cpu);
    BEQZ_Result    PARSE_BEQZ   (const int32_t instruction, MIPSCPU& cpu);
    BGE_Result     PARSE_BGE    (const int32_t instruction, MIPSCPU& cpu);
    BNE_Result     PARSE_BNE    (const int32_t instruction, MIPSCPU& cpu);
    LA_Result      PARSE_LA     (const int32_t instruction, MIPSCPU& cpu);
    LB_Result      PARSE_LB     (const int32_t instruction, MIPSCPU& cpu);
    LI_Result      PARSE_LI     (const int32_t instruction, MIPSCPU& cpu);
    SUBI_Result    PARSE_SUBI   (const int32_t instruction, MIPSCPU& cpu);
}

#endif