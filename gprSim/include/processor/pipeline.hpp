#ifndef PIPELINE_H
#define PIPELINE_H
#include "cpu.hpp"
#include "binary_parser.hpp"

using IF_RESULT = int32_t;

struct MIPSCPU;
struct ID_RESULT;

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

void IF_STAGE(MIPSCPU& cpu);
void ID_STAGE(MIPSCPU& cpu);
void EX_STAGE(MIPSCPU& cpu);
void MEM_STAGE(MIPSCPU& cpu);
void WB_STAGE(MIPSCPU& cpu);
void executeInstruction(MIPSCPU& cpu);

#endif
