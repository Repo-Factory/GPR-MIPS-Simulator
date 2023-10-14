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
#include "array_helpers.hpp"
#include "instructions.hpp"
#include "binary_parser.hpp"
#include <iostream>

#define ALL_ZEROES           0
#define NUM_FUNCTIONAL_UNITS 5
#define NUM_INSTRUCTIONS     10

constexpr const int cycleTable[NUM_INSTRUCTIONS][NUM_FUNCTIONAL_UNITS]
{   // InstructionMemory     RegisterFileRead       ALU     DataMemory      RegisterFileWrite
    {          2,                   1,               2,          0,                 1           },  // ADDI 
    {          2,                   0,               2,          0,                 0           },  // B
    {          2,                   1,               2,          0,                 0           },  // BEQZ
    {          2,                   1,               2,          0,                 0           },  // BGE
    {          2,                   1,               2,          0,                 0           },  // BNE
    {          2,                   0,               2,          0,                 2           },  // LA
    {          2,                   1,               2,          0,                 1           },  // LB
    {          2,                   0,               0,          0,                 1           },  // LI
    {          2,                   1,               2,          0,                 1           },  // SUBI
    {          2,                   1,               2,          2,                 1           }   // SYSCALL
};

const std::string functionalUnits[NUM_FUNCTIONAL_UNITS] 
{
    "InstructionMemory" ,"RegisterFileRead" ,"ALU" ,"DataMemory" ,"RegisterFileWrite"
};

int cyclesPerUnit[NUM_FUNCTIONAL_UNITS] 
{
    ALL_ZEROES   // Init Cycles for each functional unit to be 0
};

/* Called in loop from main to run program */
void executeInstruction(MIPSCPU& cpu) 
{
    const int32_t instruction = readContents(cpu.pc++);
    const int32_t opcode = BinaryParser::extractOpcode(instruction);
    switch ((opcode)) // Read instruction at PC and increment PC after             
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
    overlayArrays(cyclesPerUnit, cycleTable[opcode], NUM_FUNCTIONAL_UNITS); // Increment cycles of each functional unit
    cpu.instructionsExecuted++;                                             // Incremenent total instructions executed
}

////////////////////////////////////////////////////////////////////////////////////////////
/**** These functions will manipulate values in the registers to perform calculation ****/
////////////////////////////////////////////////////////////////////////////////////////////

#define ACCOUNT_FOR_INCREMENTED_PC(number) number-1

/* 
 * We'll use this as a constant to help store absolute addresses. If you want to store an address you can go to later
 * such as in an LA instruction, you would find the displacement from this base address and store it. Then later you 
 * would know to use this address as the base, add the offset, and locate the address you needed 
 */ 
int32_t* ABSOLUTE_BASE_ADDRESS(MIPSCPU& cpu) 
{
    return (int32_t*)&cpu.memory;           
}

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
}   // Dereferencing char pointer gives us a byte

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

///////////////////////////////////////         SYSCALL         /////////////////////////////////////////////

constexpr const char* TOTAL_INSTRUCTIONS_MESSAGE =            "Total Instructions Executed: %d\n";
constexpr const char* TOTAL_CYCLES_MESSAGE =                  "Total Cycles Taken: %d\n";
constexpr const char* SPEEDUP_MESSAGE =                       "Speedup: %f\n";
constexpr const char* CYCLES_FOR_FUNCTIONAL_UNIT_MESSAGE =    "Cycles for %s: %d\n";

constexpr const char* OUTPUT_FILE =                           "result2.txt";
constexpr const char* WRITE_MODE =                            "w";
constexpr const char* OUTPUT_MESSAGE =                        "Instructions Executed: %d\nTotal Cycles: %d\nSpeed Up: %f";

constexpr const int SPEEDUP_CONSTANT =                        8;
constexpr const float SPEEDUP(const int IC, const int C) {return SPEEDUP_CONSTANT * ((float)IC/(float)C);}

void exitProcedure(MIPSCPU& cpu)
{
    cpu.userMode = false;
    cpu.totalCycles = sumArray(cyclesPerUnit, NUM_FUNCTIONAL_UNITS);
    const float speedup = SPEEDUP(cpu.instructionsExecuted, cpu.totalCycles);
    
    printf(TOTAL_INSTRUCTIONS_MESSAGE, cpu.instructionsExecuted);
    printf(TOTAL_CYCLES_MESSAGE, cpu.totalCycles);
    printf(SPEEDUP_MESSAGE, speedup);
    
    forEachItem(cyclesPerUnit, NUM_FUNCTIONAL_UNITS, [&](int cycles, int index) {
        printf(CYCLES_FOR_FUNCTIONAL_UNIT_MESSAGE, functionalUnits[index].c_str(), cycles);
    }); // Print Cycles for each functional unit

    FILE* output = fopen(OUTPUT_FILE, WRITE_MODE);
    fprintf(output, OUTPUT_MESSAGE, cpu.instructionsExecuted, cpu.totalCycles, speedup);
    fclose(output);
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
        printf("Please Enter A String: ");
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
        exitProcedure(cpu);
    }
}
