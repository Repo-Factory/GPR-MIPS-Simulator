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
    const ADDI_Instruction addi_instruction = BinaryParser::PARSE_ADDI(instruction, cpu);
    *addi_instruction.Rdest = *addi_instruction.Rsrc1 + addi_instruction.Imm;
}

void B(const int32_t instruction, MIPSCPU& cpu)
{
    const B_Instruction b_instruction = BinaryParser::PARSE_B(instruction, cpu);
    cpu.pc += b_instruction.label;
}

void BEQZ(const int32_t instruction, MIPSCPU& cpu)
{
    const BEQZ_Instruction beqz_instruction = BinaryParser::PARSE_BEQZ(instruction, cpu);
    if (*beqz_instruction.Rsrc1 == 0) { 
        cpu.pc+=beqz_instruction.label;
    }
}

void BGE(const int32_t instruction, MIPSCPU& cpu)
{
    const BGE_Instruction bge_instruction = BinaryParser::PARSE_BGE(instruction, cpu);
    if (*bge_instruction.Rsrc1 >= *bge_instruction.Rsrc2) {
        cpu.pc+=bge_instruction.label;
    }
}

void BNE(const int32_t instruction, MIPSCPU& cpu)
{
    const BNE_Instruction bne_instruction = BinaryParser::PARSE_BNE(instruction, cpu);
    if (*bne_instruction.Rsrc1 != *bne_instruction.Rsrc2) {
        cpu.pc+=bne_instruction.label;
    }
}

void LA(const int32_t instruction, MIPSCPU& cpu)
{
    const LA_Instruction la_instruction = BinaryParser::PARSE_LA(instruction, cpu);
    *la_instruction.Rdest = la_instruction.label;
}

void LB(const int32_t instruction, MIPSCPU& cpu)
{
    const LB_Instruction lb_instruction = BinaryParser::PARSE_LB(instruction, cpu);
    *lb_instruction.Rdest = *(lb_instruction.Rsrc1 + lb_instruction.offset);
}

void LI(const int32_t instruction, MIPSCPU& cpu)
{
    const LI_Instruction li_instruction = BinaryParser::PARSE_LI(instruction, cpu);
    *li_instruction.Rdest = li_instruction.Imm;
}

void SUBI(const int32_t instruction, MIPSCPU& cpu)
{
    const SUBI_Instruction subi_instruction = BinaryParser::PARSE_SUBI(instruction, cpu);
    *subi_instruction.Rdest = *subi_instruction.Rsrc1 - subi_instruction.Imm;
}


void SYSCALL(const int32_t instruction, MIPSCPU& cpu)
{
    // const int32_t* $v0 = cpu.registerMap[RegisterTable::getRegister("$v0")];
    // const int32_t* $a0 = cpu.registerMap[RegisterTable::getRegister("$a0")];
    // std::string input;

    // if (*$v0 == 4) {
    //     printf("Please Enter A String To Check If It Is A Palindrome");
    //     std::cin >> input;
    //     int32_t* ptr = (int32_t*)&cpu.memory+*$a0;
    //     std::string* str_ptr = (std::string*)ptr;
    //     *str_ptr = input;
    // }
    // if (*$v0 == 8) {
    //     int32_t* ptr = (int32_t*)&cpu.memory+*$a0;
    //     std::cout << *(std::string*)ptr << std::endl;
    // }
    // if (*$v0 == 10) {
    //     exit(EXIT_SUCCESS);
    // }
}
