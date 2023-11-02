#ifndef CPU_H
#define CPU_H

#include "stdio.h"
#include "stdlib.h"
#include "memory.hpp"
#include "cycles.hpp"
#include <functional>

using Register          = int32_t*;
using IntegerIdentifier = int32_t;
using RegisterMap       = std::map<IntegerIdentifier, Register>;

struct MIPSCPU // CPU has memory and registers (in this case accumulator & program counter) 
{
    Memory memory;
    bool userMode = false;
    int32_t* pc = memory.userTextPtr;
    int32_t* $0 =   new int32_t();               // (zero)
    int32_t* $1 =   new int32_t();               // (at)
    int32_t* $2 =   new int32_t();               // (v0)
    int32_t* $3 =   new int32_t();               // (v1)
    int32_t* $4 =   new int32_t();               // (a0)
    int32_t* $5 =   new int32_t();               // (a1)
    int32_t* $6 =   new int32_t();               // (a2)
    int32_t* $7 =   new int32_t();               // (a3)
    int32_t* $8 =   new int32_t();               // (t0)
    int32_t* $9 =   new int32_t();               // (t1)
    int32_t* $10 =  new int32_t();               // (t2)
    int32_t* $11 =  new int32_t();               // (t3)
    int32_t* $12 =  new int32_t();               // (t4)
    int32_t* $13 =  new int32_t();               // (t5)
    int32_t* $14 =  new int32_t();               // (t6)
    int32_t* $15 =  new int32_t();               // (t7)
    int32_t* $16 =  new int32_t();               // (s0)
    int32_t* $17 =  new int32_t();               // (s1)
    int32_t* $18 =  new int32_t();               // (s2)
    int32_t* $19 =  new int32_t();               // (s3)
    int32_t* $20 =  new int32_t();               // (s4)
    int32_t* $21 =  new int32_t();               // (s5)
    int32_t* $22 =  new int32_t();               // (s6)
    int32_t* $23 =  new int32_t();               // (s7)
    int32_t* $24 =  new int32_t();               // (t8)
    int32_t* $25 =  new int32_t();               // (t9)
    int32_t* $26 =  new int32_t();               // (k0)
    int32_t* $27 =  new int32_t();               // (k1)
    int32_t* $28 =  new int32_t();               // (gp)
    int32_t* $29 =  new int32_t();               // (sp)
    int32_t* $30 =  new int32_t();               // (fp)
    int32_t* $31 =  new int32_t();               // (ra)
    const std::function<int*(bool, const int*)> cyclesPerUnit = cyclesPerUnitClosure();
    const std::function<std::vector<std::vector<int>>()> cyclesTable = cyclesTableClosure();
    const std::function<std::vector<std::string>()> functionalUnits = functionalUnitsClosure();
    const std::function<int(bool)> instructionsExecuted = instructionsExecutedClosure();
    RegisterMap registerMap
    {
        {0, $0},      {1, $1},      {2, $2},      {3, $3},     
        {4, $4},      {5, $5},      {6, $6},      {7, $7},    
        {8, $8},      {9, $9},      {10, $10},    {11, $11},    
        {12, $12},    {13, $13},    {14, $14},    {15, $15},    
        {16, $16},    {17, $17},    {18, $18},    {19, $19},    
        {20, $20},    {21, $21},    {22, $22},    {23, $23},    
        {24, $24},    {25, $25},    {26, $26},    {27, $27},    
        {28, $28},    {29, $29},    {30, $30},    {31, $31},    
    };
};

/* Functions which implement each instruction in simulator */
void ADDI(const int32_t instruction, MIPSCPU& cpu);
void B(const int32_t instruction, MIPSCPU& cpu);
void BEQZ(const int32_t instruction, MIPSCPU& cpu);
void BGE(const int32_t instruction, MIPSCPU& cpu);
void BNE(const int32_t instruction, MIPSCPU& cpu);
void LA(const int32_t instruction, MIPSCPU& cpu);
void LB(const int32_t instruction, MIPSCPU& cpu);
void LI(const int32_t instruction, MIPSCPU& cpu);
void SUBI(const int32_t instruction, MIPSCPU& cpu);
void SYSCALL(const int32_t instruction, MIPSCPU& cpu);

/* Primary function called in loop from main to simulate machine */
void executeInstruction(MIPSCPU& cpu);

#endif