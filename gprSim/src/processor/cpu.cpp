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
#include "pipeline.hpp"
#include "cycles.hpp"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
/**** These functions will manipulate values in the registers to perform calculation ****/
////////////////////////////////////////////////////////////////////////////////////////////
#define GET 0
#define ACCOUNT_FOR_INCREMENTED_PC(number) number-1

/* 
 * We'll use this as a constant to help store absolute addresses. If you want to store an address you can go to later
 * such as in an LA instruction, you would find the displacement from this base address and store it. Then later you 
 * would know to use this address as the base, add the offset, and locate the address you needed 
 */ 
int32_t* ABSOLUTE_BASE_ADDRESS(MIPSCPU& cpu) 
{
    return cpu.BASE;           
}

EX_Result ADDI(const ADDI_Result addi_instruction)
{
    return EX_Result{MIPS_TYPE::RType, addi_instruction.Rdest, *addi_instruction.Rsrc1 + addi_instruction.Imm};
}

// For branch instructions we can move the PC by the offset indicated in the label
EX_Result B(const B_Result b_instruction, MIPSCPU cpu)
{
    return EX_Result{MIPS_TYPE::JType, NULL, 0};
}

EX_Result BEQZ(const BEQZ_Result beqz_instruction, MIPSCPU cpu)
{
    return EX_Result{MIPS_TYPE::JType, NULL, 0};
}

EX_Result BGE(const BGE_Result bge_instruction, MIPSCPU cpu)
{
    return EX_Result{MIPS_TYPE::JType, NULL, 0};
}

EX_Result BNE(const BNE_Result bne_instruction, MIPSCPU cpu)
{
    return EX_Result{MIPS_TYPE::JType, NULL, 0};
}

// Here the label that was stored is relative to the pc counter (LOCCTR). However, we have to store an absolute address because
// if we jump from the PC the offset will have changed at a new instruction. We can use the absolute base address to 
// jump from an absolute reference point. However, we don't explicitly know how far the target is from the absolute base address.
// We do know how far it is from the PC. So we can find how far the pc is from the base address, add that to how far the address
// is from the PC (stored in the label), and we will have the distance from the absolute base address stored in the register
EX_Result LA(const LA_Result la_instruction, MIPSCPU cpu)  // Address is stored as offset from base
{    
    const long offsetBetweenPcAndBase = cpu.pc - ABSOLUTE_BASE_ADDRESS(cpu);
    const int32_t pcRelativeLabelOffset = ACCOUNT_FOR_INCREMENTED_PC(la_instruction.label);
    const long baseRelativeAddress = offsetBetweenPcAndBase + pcRelativeLabelOffset;  // Label is stored as offset from PC
    return EX_Result{MIPS_TYPE::IType, la_instruction.Rdest, baseRelativeAddress};
}   // Use Fixed Address &cpu.memory to convert to absolute address.

// Here we want to dereference a byte of data that is found out a certain distance from the source register.
EX_Result LB(const LB_Result lb_instruction, MIPSCPU cpu)
{
    const int32_t* offsetAddress = lb_instruction.Rsrc1 + lb_instruction.offset;
    const long offsetAddressContents = *offsetAddress;
    const int32_t* byteAddress = ABSOLUTE_BASE_ADDRESS(cpu) + offsetAddressContents;
    const long byteAddressContents = *(char*)byteAddress;
    return EX_Result{MIPS_TYPE::IType, lb_instruction.Rdest, byteAddressContents}; //
}   // Dereferencing char pointer gives us a byte

EX_Result LI(const LI_Result li_instruction)
{
    return EX_Result{MIPS_TYPE::IType, li_instruction.Rdest, li_instruction.Imm};
}

EX_Result SUBI(const SUBI_Result subi_instruction)
{
    return EX_Result{MIPS_TYPE::RType, subi_instruction.Rdest, *subi_instruction.Rsrc1 - subi_instruction.Imm};
}

///////////////////////////////////////         SYSCALL         /////////////////////////////////////////////

constexpr const char* TOTAL_INSTRUCTIONS_MESSAGE =            "Total Instructions Executed: %d\n";
constexpr const char* TOTAL_CYCLES_MESSAGE =                  "Total Cycles Taken: %d\n";
constexpr const char* SPEEDUP_MESSAGE =                       "Speedup: %f\n";
constexpr const char* CYCLES_FOR_FUNCTIONAL_UNIT_MESSAGE =    "Cycles for %s: %d\n";
constexpr const char* NOP_COUNT_MESSAGE =                     "Number of NOP Instructions: %d\n";

constexpr const char* OUTPUT_FILE =                           "result2.txt";
constexpr const char* WRITE_MODE =                            "w";
constexpr const char* OUTPUT_MESSAGE =                        "Instructions Executed: %d\nTotal Cycles: %d\nSpeed Up: %f";

constexpr const int SPEEDUP_CONSTANT =                        8;
constexpr const float SPEEDUP(const int IC, const int C)      {return SPEEDUP_CONSTANT * ((float)IC/(float)C);}

// Print statistics and exit CPU loop
void exitProcedure(MIPSCPU& cpu)
{
    cpu.userMode = false;
    const int totalCycles = sumArray(cpu.cyclesPerUnit(GET, nullptr), NUM_FUNCTIONAL_UNITS);
    const int instructionsExecuted = cpu.instructionsExecuted(GET);
    const float speedup = SPEEDUP(instructionsExecuted, totalCycles);
    
    printf(TOTAL_INSTRUCTIONS_MESSAGE, instructionsExecuted);
    printf(TOTAL_CYCLES_MESSAGE, totalCycles);
    printf(SPEEDUP_MESSAGE, speedup);
    
    forEachItem(cpu.cyclesPerUnit(GET, nullptr), NUM_FUNCTIONAL_UNITS, [&](int cycles, int index) {
        printf(CYCLES_FOR_FUNCTIONAL_UNIT_MESSAGE, functionalUnitsNamesClosure()()[index].c_str(), cycles);
    }); // Print Cycles for each functional unit
    printf(NOP_COUNT_MESSAGE, cpu.nopCount(GET));

    FILE* output = fopen(OUTPUT_FILE, WRITE_MODE);
    fprintf(output, OUTPUT_MESSAGE, instructionsExecuted, totalCycles, speedup);
    fclose(output);
}

namespace // SYSCALL HELPERS
{
    void printStringAtAddress(const int32_t* address, MIPSCPU& cpu)
    {
        std::string* ptr = (std::string*)(ABSOLUTE_BASE_ADDRESS(cpu) + (*(address)-1));
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

void SYSCALL(MIPSCPU& cpu)
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
