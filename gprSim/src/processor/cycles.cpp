#include "cycles.hpp"
#include "array_helpers.hpp"

#define ALL_ZEROES           0

std::function<int*(bool, const int*)> cyclesPerUnitClosure()
{
    int cyclesPerUnit[NUM_FUNCTIONAL_UNITS] {ALL_ZEROES};   // Init Cycles for each functional unit to be 0
    return [=](bool set, const int* row) mutable {   
        if (set) overlayArrays(cyclesPerUnit, row, NUM_FUNCTIONAL_UNITS); // Increment cycles of each functional unit
        return cyclesPerUnit;   
    };
}

std::function<std::vector<std::vector<int>>()> cyclesTableClosure()
{
    const std::vector<std::vector<int>> cycleTable
    {   // InstructionMemory     RegisterFileRead       ALU     DataMemory      RegisterFileWrite
        {          0,                   0,               0,          0,                 0           },  // NOP
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
    return [=]() {   
        return cycleTable;   
    };
}

const std::function<std::vector<std::string>()> functionalUnitsNamesClosure()
{
    const std::vector<std::string> functionalUnits 
    {
        "InstructionMemory" ,"RegisterFileRead" ,"ALU" ,"DataMemory" ,"RegisterFileWrite"
    };
    return [=]() {   
        return functionalUnits;  
    };
}

const std::function<int(bool)> instructionsExecutedClosure()
{
    int instructionsExecuted = 0;   // Init Cycles for each functional unit to be 0
    return [=](const bool set) mutable {   
        return set ? instructionsExecuted++ : instructionsExecuted;   
    };
}

