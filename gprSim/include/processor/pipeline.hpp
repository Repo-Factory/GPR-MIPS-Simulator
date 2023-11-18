#ifndef PIPELINE_H
#define PIPELINE_H
#include "cpu.hpp"
#include "binary_parser.hpp"

using IF_RESULT = int32_t;

struct ID_RESULT
{
    Instruction type;
    union 
    {
        ADDI_Result ADDI_instruction;
        B_Result B_instruction;
        BEQZ_Result BEQZ_instruction;
        BGE_Result BGE_instruction;
        BNE_Result BNE_instruction;
        LA_Result LA_instruction;
        LB_Result LB_instruction;
        LI_Result LI_instruction;
        SUBI_Result SUBI_instruction;
    };
};

struct EX_Result
{
    MIPS_TYPE type;
    int32_t* destination;
    long result;
};

struct MEM_Result
{
    int32_t* destination;
    long result;
};

struct PipelineLatches
{
    IF_RESULT IF_ID_LATCH;
    ID_RESULT ID_EX_LATCH;
    EX_Result EX_MEM_LATCH;
    MEM_Result MEM_WB_LATCH;
};

enum class Stage
{
    IF_STAGE =  0,
    ID_STAGE =  1,
    EX_STAGE =  2,
    MEM_STAGE = 3,
    WB_STAGE =  4,
};

struct MIPSCPU;
void IF_STAGE(MIPSCPU& cpu);
void ID_STAGE(MIPSCPU& cpu);
void EX_STAGE(MIPSCPU& cpu);
void MEM_STAGE(MIPSCPU& cpu);
void WB_STAGE(MIPSCPU& cpu);
void clockCycle(MIPSCPU& cpu);

#endif
