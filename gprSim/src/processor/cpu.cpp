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
#include <iostream>

constexpr const int NON_OPCODE_BITS = 32-6;

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
    std::cout << (instruction >> NON_OPCODE_BITS) << std::endl;
}

/* Called in loop from main to run program */
void executeInstruction(MIPSCPU& cpu) 
{
    switch ((extractOpcode(readContents(cpu.pc++)))) // Read instruction at PC and increment PC after             
    {
        case ADDI_OPCODE():
             ADDI(cpu);
             break;
        case B_OPCODE():
             B(cpu);
             break;
        case BEQZ_OPCODE():
             BEQZ(cpu);
             break;
        case BGE_OPCODE():
             BGE(cpu);
             break;
        case BNE_OPCODE():
             BNE(cpu);
             break;
        case LA_OPCODE():
             LA(cpu);
             break;
        case LB_OPCODE():
             LB(cpu);
             break;
        case LI_OPCODE():
             LI(cpu);
             break;
        case SUBI_OPCODE():
             SUBI(cpu);
             break;
        case SYSCALL_OPCODE():
             SYSCALL(cpu);
             break;
    }
    if (cpu.pc == cpu.memory.userTextPtr + 27) exit(0);
}


////////////////////////////////////////////////////////////////////////////////////////////
/**** These functions will manipulate values in the accumulator to perform calculation ****/
////////////////////////////////////////////////////////////////////////////////////////////

void ADDI(MIPSCPU& cpu)
{

}

void B(MIPSCPU& cpu)
{

}

void BEQZ(MIPSCPU& cpu)
{

}

void BGE(MIPSCPU& cpu)
{

}

void BNE(MIPSCPU& cpu)
{

}

void LA(MIPSCPU& cpu)
{

}

void LB(MIPSCPU& cpu)
{

}

void LI(MIPSCPU& cpu)
{

}

void SUBI(MIPSCPU& cpu)
{

}

void SYSCALL(MIPSCPU& cpu)
{

}
