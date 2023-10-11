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
    int32_t* $0 =  NULL;    // (zero)
    int32_t* $1 =  NULL;    // (at)
    int32_t* $2 =  NULL;    // (v0)
    int32_t* $3 =  NULL;    // (v1)
    int32_t* $4 =  NULL;    // (a0)
    int32_t* $5 =  NULL;    // (a1)
    int32_t* $6 =  NULL;    // (a2)
    int32_t* $7 =  NULL;    // (a3)
    int32_t* $8 =  NULL;    // (t0)
    int32_t* $9 =  NULL;    // (t1)
    int32_t* $10 = NULL;    // (t2)
    int32_t* $11 = NULL;    // (t3)
    int32_t* $12 = NULL;    // (t4)
    int32_t* $13 = NULL;    // (t5)
    int32_t* $14 = NULL;    // (t6)
    int32_t* $15 = NULL;    // (t7)
    int32_t* $16 = NULL;    // (s0)
    int32_t* $17 = NULL;    // (s1)
    int32_t* $18 = NULL;    // (s2)
    int32_t* $19 = NULL;    // (s3)
    int32_t* $20 = NULL;    // (s4)
    int32_t* $21 = NULL;    // (s5)
    int32_t* $22 = NULL;    // (s6)
    int32_t* $23 = NULL;    // (s7)
    int32_t* $24 = NULL;    // (t8)
    int32_t* $25 = NULL;    // (t9)
    int32_t* $26 = NULL;    // (k0)
    int32_t* $27 = NULL;    // (k1)
    int32_t* $28 = NULL;    // (gp)
    int32_t* $29 = NULL;    // (sp)
    int32_t* $30 = NULL;    // (fp)
    int32_t* $31 = NULL;    // (ra)
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