#ifndef PIPELINE_H
#define PIPELINE_H
#include "cpu.hpp"
#include "binary_parser.hpp"

struct MIPSCPU;
struct InstructionType;

struct PipelineLatches
{
    int32_t IF_ID_LATCH;
    InstructionType ID_EX_LATCH;
    int32_t EX_MEM_LATCH;
    int32_t MEM_WB_LATCH;
};

enum class Stage
{
    IF_STAGE =  0,
    ID_STAGE =  1,
    EX_STAGE =  2,
    MEM_STAGE = 3,
    WB_STAGE =  4,
};

void IF_STAGE(MIPSCPU& cpu);
void ID_STAGE(MIPSCPU& cpu);
void EX_STAGE(MIPSCPU& cpu);
void MEM_STAGE(MIPSCPU& cpu);
void WB_STAGE(MIPSCPU& cpu);
void executeInstruction(MIPSCPU& cpu);

#endif
