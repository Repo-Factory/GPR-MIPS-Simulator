/*
 *  @brief
 *          CPU related functions to execute instructions
 *
 * CPU will continually read contents at program counter
 * If the contents are a recognized instruction, it will execute
 * the appropriate program logic
 */

#include "stdio.h"
#include "stdlib.h"
#include "cpu.hpp"
#include "memory.hpp"
#include "debug_helpers.hpp"
#include "instructions.hpp"
#include "binary_parser.hpp"
#include <iostream>

constexpr const int INSTRUCTION_SIZE    = 32;
constexpr const int OPCODE_BITS         = 6; 
constexpr const int NON_OPCODE_BITS     = INSTRUCTION_SIZE-OPCODE_BITS;

int CycleTable[10][5] = 
{
    {2,1,2,0,1},
    {2,0,2,0,0},
    {2,1,2,0,0},
    {2,1,2,0,0},
    {2,1,2,0,0},
    {2,0,2,0,2},
    {2,1,2,0,1},
    {2,0,0,0,1},
    {2,1,2,0,1},
    {2,1,2,2,1}
};

int32_t extractOpcode(const int32_t instruction)
{
    return (instruction >> NON_OPCODE_BITS);
}

/* Called in loop from main to run program */
void executeInstruction(MIPSCPU& cpu) 
{
    const int32_t instruction = readContents(cpu.pc++);
    switch ((extractOpcode(instruction))) // Read instruction at PC and increment PC after             
    {
        case ADDI_OPCODE():
             ADDI(instruction, cpu);
             break;
        case B_OPCODE():
             B(instruction, cpu);
             break;
        case BEQZ_OPCODE():
             BEQZ(instruction, cpu);
             break;
        case BGE_OPCODE():
             BGE(instruction, cpu);
             break;
        case BNE_OPCODE():
             BNE(instruction, cpu);
             break;
        case LA_OPCODE():
             LA(instruction, cpu);
             break;
        case LB_OPCODE():
             LB(instruction, cpu);
             break;
        case LI_OPCODE():
             LI(instruction, cpu);
             break;
        case SUBI_OPCODE():
             SUBI(instruction, cpu);
             break;
        case SYSCALL_OPCODE():
             SYSCALL(instruction, cpu);
             break;
    }
    if (cpu.pc == cpu.memory.userTextPtr + 27) exit(0);
}


////////////////////////////////////////////////////////////////////////////////////////////
/**** These functions will manipulate values in the accumulator to perform calculation ****/
////////////////////////////////////////////////////////////////////////////////////////////


void ADDI(const int32_t instruction, MIPSCPU& cpu)
{
    const ADDI_Instruction addi_instruction = PARSE_ADDI(instruction, cpu);
    *addi_instruction.Rdest = *addi_instruction.Rsrc1 + addi_instruction.Imm;
}

void B(const int32_t instruction, MIPSCPU& cpu)
{
    const B_Instruction b_instruction = PARSE_B(instruction, cpu);
    cpu.pc += b_instruction.label;
}

void BEQZ(const int32_t instruction, MIPSCPU& cpu)
{
    const BEQZ_Instruction beqz_instruction = PARSE_BEQZ(instruction, cpu);
    if (*beqz_instruction.Rsrc1 == 0) { 
        cpu.pc+=beqz_instruction.label;
    }
}

void BGE(const int32_t instruction, MIPSCPU& cpu)
{
    const BGE_Instruction bge_instruction = PARSE_BGE(instruction, cpu);
    if (*bge_instruction.Rsrc1 >= *bge_instruction.Rsrc2) {
        cpu.pc+=bge_instruction.label;
    }
}

void BNE(const int32_t instruction, MIPSCPU& cpu)
{
    const BNE_Instruction bne_instruction = PARSE_BNE(instruction, cpu);
    if (*bne_instruction.Rsrc1 != *bne_instruction.Rsrc2) {
        cpu.pc+=bne_instruction.label;
    }
}

void LA(const int32_t instruction, MIPSCPU& cpu)
{
    


}

void LB(const int32_t instruction, MIPSCPU& cpu)
{

}

void LI(const int32_t instruction, MIPSCPU& cpu)
{

}

void SUBI(const int32_t instruction, MIPSCPU& cpu)
{

}

void SYSCALL(const int32_t instruction, MIPSCPU& cpu)
{

}
