#include "binary_parser.hpp"
#include "cpu.hpp"

#define ACCOUNT_FOR_INCREMENTED_PC(number) number-1

constexpr const int BIT                 = 1;
constexpr const int INSTRUCTION_SIZE    = 32;
constexpr const int OPCODE_BITS         = 6; 
constexpr const int IMMEDIATE_BITS      = 16;
constexpr const int OFFSET_BITS         = 16;
constexpr const int REGISTER_BITS       = 5; 
constexpr const int NON_OPCODE_BITS     = INSTRUCTION_SIZE-OPCODE_BITS;

int32_t fixSignedness(const int x, const int shift)
{
    return ((x << shift) >> shift);
}

int32_t xOnes(const int x) // Generate Bit Stream of X number of Ones
{
    return ((BIT << x) - 1); // -1 flips all bits behind the 1
}

int32_t BinaryParser::extractOpcode(const int32_t instruction)
{
    return (instruction >> NON_OPCODE_BITS);
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

ADDI_Result BinaryParser::PARSE_ADDI(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_dest_identifier   = instruction >> 21  & xOnes(REGISTER_BITS);
    const int32_t r_src_identifier    = instruction >> 16  & xOnes(REGISTER_BITS);
    const int32_t imm                 = instruction >> 0   & xOnes(IMMEDIATE_BITS);
    return ADDI_Result           {cpu.registerMap[r_dest_identifier], cpu.registerMap[r_src_identifier], imm};
}

B_Result BinaryParser::PARSE_B(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t label               = fixSignedness(instruction >> 0 & xOnes(26), 6);   // Correct for signedness
    cpu.pc += ACCOUNT_FOR_INCREMENTED_PC(label);
    return B_Result              {label};
}

BEQZ_Result BinaryParser::PARSE_BEQZ(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_src_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t label              = fixSignedness(instruction >> 0 & xOnes(21), 11);
    if (*cpu.registerMap[r_src_identifier] == 0) { 
        cpu.pc+=ACCOUNT_FOR_INCREMENTED_PC(label);
    }
    return BEQZ_Result          {cpu.registerMap[r_src_identifier], label};
}

BGE_Result BinaryParser::PARSE_BGE(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_src_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t r_src2_identifier  = instruction >> 16    & xOnes(REGISTER_BITS);
    const int32_t label              = fixSignedness(instruction >> 0 & xOnes(16), 16);
    if (*cpu.registerMap[r_src_identifier] >= *cpu.registerMap[r_src2_identifier]) {
        cpu.pc+=ACCOUNT_FOR_INCREMENTED_PC(label);
    }
    return BGE_Result           {cpu.registerMap[r_src_identifier], cpu.registerMap[r_src2_identifier], label};
}

BNE_Result BinaryParser::PARSE_BNE(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_src_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t r_src2_identifier  = instruction >> 16    & xOnes(REGISTER_BITS);
    const int32_t label              = fixSignedness(instruction >> 0 & xOnes(16), 16);
    if (*cpu.registerMap[r_src_identifier] != *cpu.registerMap[r_src2_identifier]) {
        cpu.pc+=ACCOUNT_FOR_INCREMENTED_PC(label);
    }
    return BNE_Result           {cpu.registerMap[r_src_identifier], cpu.registerMap[r_src2_identifier], label};
}

LA_Result BinaryParser::PARSE_LA(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_src_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t label              = instruction >> 0     & xOnes(21);
    return LA_Result            {cpu.registerMap[r_src_identifier], label};
}

LB_Result BinaryParser::PARSE_LB(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_dest_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t r_src_identifier    = instruction >> 16    & xOnes(REGISTER_BITS);
    const int32_t offset              = instruction >> 0     & xOnes(OFFSET_BITS);
    return LB_Result             {cpu.registerMap[r_dest_identifier], cpu.registerMap[r_src_identifier], offset};
}

LI_Result BinaryParser::PARSE_LI(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_dest_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t imm                 = instruction >> 0     & xOnes(IMMEDIATE_BITS);
    return LI_Result             {cpu.registerMap[r_dest_identifier], imm};
}

SUBI_Result BinaryParser::PARSE_SUBI(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t r_dest_identifier   = instruction >> 21    & xOnes(REGISTER_BITS);
    const int32_t r_src_identifier    = instruction >> 16    & xOnes(REGISTER_BITS);
    const int32_t imm                 = instruction >> 0     & xOnes(IMMEDIATE_BITS);
    return SUBI_Result           {cpu.registerMap[r_dest_identifier], cpu.registerMap[r_src_identifier], imm};
}
