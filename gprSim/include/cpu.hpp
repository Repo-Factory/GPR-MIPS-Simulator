#ifndef CPU_H
#define CPU_H

#include "stdio.h"
#include "stdlib.h"
#include "memory.hpp"
#include "parser.hpp"

struct MIPSCPU // CPU has memory and registers (in this case accumulator & program counter) 
{
    Memory memory;
    bool userMode = false;
    int32_t* accumulator = new int();
    int32_t* pc = memory.userTextPtr;
    int32_t* $0 =  NULL;
    int32_t* $1 =  NULL;
    int32_t* $2 =  NULL;
    int32_t* $3 =  NULL;
    int32_t* $4 =  NULL;
    int32_t* $5 =  NULL;
    int32_t* $6 =  NULL;
    int32_t* $7 =  NULL;
    int32_t* $8 =  NULL;
    int32_t* $9 =  NULL;
    int32_t* $10 = NULL;
    int32_t* $11 = NULL;
    int32_t* $12 = NULL;
    int32_t* $13 = NULL;
    int32_t* $14 = NULL;
    int32_t* $15 = NULL;
    int32_t* $16 = NULL;
    int32_t* $17 = NULL;
    int32_t* $18 = NULL;
    int32_t* $19 = NULL;
    int32_t* $20 = NULL;
    int32_t* $21 = NULL;
    int32_t* $22 = NULL;
    int32_t* $23 = NULL;
    int32_t* $24 = NULL;
    int32_t* $25 = NULL;
    int32_t* $26 = NULL;
    int32_t* $27 = NULL;
    int32_t* $28 = NULL;
    int32_t* $29 = NULL;
    int32_t* $30 = NULL;
    int32_t* $31 = NULL;  
};

/* Functions which implement each instruction in simulator */
void load(MIPSCPU& cpu);
void store(MIPSCPU& cpu);
void add(MIPSCPU& cpu);
void mult(MIPSCPU& cpu);
void end(MIPSCPU& cpu);

/* Primary function called in loop from main to simulate machine */
void executeInstruction(MIPSCPU& cpu);


enum class FunctionalUnits
{
    InstructionMemory               = 0, 
    RegisterFileRead                = 1, 
    ALU                             = 2, 
    Data                            = 3, 
    memory                          = 4,
    RegisterFileWrite               = 5
};


#endif

/* ADDI Rdest, Rsrc1, Imm
B label
BEQZ Rsrc1, label
BGE Rsrc1, Rsrc2, label
BNE Rsrc1, Rsrc2, label
LA Rdest, label
3
LB Rdest, offset(Rsrc1)
LI Rdest, Imm 
SUBI Rdest, Rsrc1, Imm
SYSCALL */