#include "binary_parser.hpp"

constexpr const int IMMEDIATE_BITS    = 16;
constexpr const int OFFSET_BITS       = 16;
constexpr const int REGISTER_BITS     = 5; 

int32_t xOnes(const int x) // Generate Bit Stream of X number of Ones
{
    return ((1 << x) - 1);
}

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

ADDI_Instruction BinaryParser::PARSE_ADDI(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_dest_identifier   = instruction >> 21  & xOnes(REGISTER_BITS);
    const int32_t r_src_identifier    = instruction >> 16  & xOnes(REGISTER_BITS);
    const int32_t imm                 = instruction >> 0   & xOnes(IMMEDIATE_BITS);
    return ADDI_Instruction           {cpu.registerMap[r_dest_identifier], cpu.registerMap[r_src_identifier] , imm};
}

B_Instruction BinaryParser::PARSE_B(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t label               = instruction >> 0 & xOnes(26);
    return B_Instruction              {label};
}

BEQZ_Instruction BinaryParser::PARSE_BEQZ(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_src_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t label              = instruction >> 0     & xOnes(21);
    return BEQZ_Instruction          {cpu.registerMap[r_src_identifier], label};
}

BGE_Instruction BinaryParser::PARSE_BGE(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_src_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t r_src2_identifier  = instruction >> 16    & xOnes(REGISTER_BITS);
    const int32_t label              = instruction >> 0     & xOnes(16);
    return BGE_Instruction           {cpu.registerMap[r_src_identifier], cpu.registerMap[r_src2_identifier], label};
}

BNE_Instruction BinaryParser::PARSE_BNE(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_src_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t r_src2_identifier  = instruction >> 16    & xOnes(REGISTER_BITS);
    const int32_t label              = instruction >> 0     & xOnes(16);
    return BNE_Instruction{cpu.registerMap[r_src_identifier], cpu.registerMap[r_src2_identifier], label};
}

LA_Instruction BinaryParser::PARSE_LA(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_src_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t label              = instruction >> 0     & xOnes(21);
    return LA_Instruction{cpu.registerMap[r_src_identifier], label};
}

LB_Instruction BinaryParser::PARSE_LB(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_dest_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t r_src_identifier    = instruction >> 16    & xOnes(REGISTER_BITS);
    const int32_t offset              = instruction >> 0     & xOnes(OFFSET_BITS);
    return LB_Instruction{cpu.registerMap[r_dest_identifier], cpu.registerMap[r_src_identifier], offset};
}

LI_Instruction BinaryParser::PARSE_LI(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_dest_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t imm                 = instruction >> 0   & xOnes(IMMEDIATE_BITS);
    return LI_Instruction{cpu.registerMap[r_dest_identifier], imm};
}

SUBI_Instruction BinaryParser::PARSE_SUBI(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_dest_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t r_src_identifier    = instruction >> 16    & xOnes(REGISTER_BITS);
    const int32_t imm                 = instruction >> 0     & xOnes(IMMEDIATE_BITS);
    return SUBI_Instruction{cpu.registerMap[r_dest_identifier], cpu.registerMap[r_src_identifier], imm};
}
