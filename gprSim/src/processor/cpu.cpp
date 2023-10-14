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

#define ACCOUNT_FOR_INCREMENTED_PC(number) number-1
int32_t* ABSOLUTE_BASE_ADDRESS(MIPSCPU& cpu) {return (int32_t*)&cpu.memory;}

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

/* Called in loop from main to run program */
void executeInstruction(MIPSCPU& cpu) 
{
    const int32_t instruction = readContents(cpu.pc++);
    switch ((BinaryParser::extractOpcode(instruction))) // Read instruction at PC and increment PC after             
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
}


////////////////////////////////////////////////////////////////////////////////////////////
/**** These functions will manipulate values in the registers to perform calculation ****/
////////////////////////////////////////////////////////////////////////////////////////////


void ADDI(const int32_t instruction, MIPSCPU& cpu)
{
    const ADDI_Instruction addi_instruction = BinaryParser::PARSE_ADDI(instruction, cpu);
    *addi_instruction.Rdest = *addi_instruction.Rsrc1 + addi_instruction.Imm;
}

void B(const int32_t instruction, MIPSCPU& cpu)
{
    const B_Instruction b_instruction = BinaryParser::PARSE_B(instruction, cpu);
    cpu.pc += ACCOUNT_FOR_INCREMENTED_PC(b_instruction.label);
}

void BEQZ(const int32_t instruction, MIPSCPU& cpu)
{
    const BEQZ_Instruction beqz_instruction = BinaryParser::PARSE_BEQZ(instruction, cpu);
    if (*beqz_instruction.Rsrc1 == 0) { 
        cpu.pc+=ACCOUNT_FOR_INCREMENTED_PC(beqz_instruction.label);
    }
}

void BGE(const int32_t instruction, MIPSCPU& cpu)
{
    const BGE_Instruction bge_instruction = BinaryParser::PARSE_BGE(instruction, cpu);
    if (*bge_instruction.Rsrc1 >= *bge_instruction.Rsrc2) {
        cpu.pc+=ACCOUNT_FOR_INCREMENTED_PC(bge_instruction.label);
    }
}

void BNE(const int32_t instruction, MIPSCPU& cpu)
{
    const BNE_Instruction bne_instruction = BinaryParser::PARSE_BNE(instruction, cpu);
    if (*bne_instruction.Rsrc1 != *bne_instruction.Rsrc2) {
        cpu.pc+=ACCOUNT_FOR_INCREMENTED_PC(bne_instruction.label);
    }
}

void LA(const int32_t instruction, MIPSCPU& cpu)
{
    const LA_Instruction la_instruction = BinaryParser::PARSE_LA(instruction, cpu);
    *la_instruction.Rdest = (cpu.pc - ABSOLUTE_BASE_ADDRESS(cpu)) + ACCOUNT_FOR_INCREMENTED_PC(la_instruction.label); // Use Fixed Address &cpu.memory to convert to absolute address
}

void LB(const int32_t instruction, MIPSCPU& cpu)
{
    const LB_Instruction lb_instruction = BinaryParser::PARSE_LB(instruction, cpu);
    *lb_instruction.Rdest = *(char*)(ABSOLUTE_BASE_ADDRESS(cpu) + *(lb_instruction.Rsrc1 + lb_instruction.offset));
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

namespace // SYSCALL HELPERS
{
    void printStringAtAddress(const int32_t* address, MIPSCPU& cpu)
    {
        std::string* ptr = (std::string*)(ABSOLUTE_BASE_ADDRESS(cpu) + (*address));
        std::cout << *ptr << std::endl;
    }

    std::string getInputString()
    {
        std::string input;
        printf("Please Enter A String To Check If It Is A Palindrome: ");
        std::cin >> input;
        return input;
    }

    void storeStringIntoAddress(const int32_t* address, MIPSCPU& cpu)
    {
        const char* input_string = getInputString().c_str();
        int32_t* space_allocated = ABSOLUTE_BASE_ADDRESS(cpu) + *address;
        while (*input_string != '\0') { // Iterate each char of input string and store them to previously allocated address
            *space_allocated = *input_string;
            space_allocated++; 
            input_string++;
        }
    }
}

constexpr const int PRINT_STRING_CODE  = 4;
constexpr const int STORE_STRING_CODE  = 8;
constexpr const int EXIT_CODE          = 10;

void SYSCALL(const int32_t instruction, MIPSCPU& cpu)
{
    const int32_t* $v0 = cpu.registerMap[RegisterTable::getRegister("$v0")];
    const int32_t* $a0 = cpu.registerMap[RegisterTable::getRegister("$a0")];
    if (*$v0 == PRINT_STRING_CODE) {
        printStringAtAddress($a0, cpu);
    }
    if (*$v0 == STORE_STRING_CODE) {
        storeStringIntoAddress($a0, cpu);
    }
    if (*$v0 == EXIT_CODE) {
        exit(EXIT_SUCCESS);
    }
}
