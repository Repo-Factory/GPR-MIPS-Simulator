/* 
 * @brief 
 *
 * 
 */
#include "cpu.hpp"
#include "pipeline.hpp"
#include "instructions.hpp"
#include "binary_parser.hpp"
#include <cstdint>

#define SET 1

/* The fetch stage is simple: just use the current value in the PC to index memory, and retrieve the instruction you find in the memory. Put it in a
buffer (you could call it the IF/ID latch!) where it will be grabbed by decode in the next clock cycle. Note that you have to make sure that the
decode stage doesn't get the instruction for cycle (N+1) by mistake.
2. Decode reads zero, one or two values out of the register file and stores them in a buffer (the ID/EX latch ?) for use by the execute stage in the
next clock cycle (we're not working directly with binary encoding of the instructions, so this stage can be a little smarter than the real hardware).
If the current instruction is a branch, this stage needs to decide whether or not the branch is taken, and update the PC accordingly. Please keep in
mind that this updated PC shouldn't be used until the next cycle, so if you're using the "pull" model you'll need to delay the change of PC until
then.
3. Execute behaves as necessary, either completing an R-type instruction (such as an add) or doing an address calculation for a load or a store. The
result could be put in an EX/MEM latch.
4. The memory stage references data memory, performing a read or a write if the instruction in the stage is a load or a store, respectively. If the
instruction in MEM is an R-type instruction, the MEM/WB latch should buffer the result that was being held in the EX/MEM latch.
5. Write-back puts the results of an R-type instruction or a load into the register file. This value should come from the MEM/WB latch. */

int32_t IF_ID_LATCH;
int32_t ID_EX_LATCH;
int32_t EX_MEM_LATCH;
int32_t MEM_WB_LATCH;

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
    cpu.cyclesPerUnit(SET, cpu.cyclesTable()[opcode].data());
    cpu.instructionsExecuted(SET);                                           // Incremenent total instructions executed
}


void IF_STAGE(MIPSCPU& cpu) 
{

}

void ID_STAGE(MIPSCPU& cpu) 
{

}

void EX_STAGE(MIPSCPU& cpu) 
{

}

void MEM_STAGE(MIPSCPU& cpu) 
{

}

void WB_STAGE(MIPSCPU& cpu) 
{

}



