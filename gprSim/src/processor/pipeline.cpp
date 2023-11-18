/* 
 * @brief Define pipeline stages for CPU
 *
 * CPU instructions won't be executed in one cycle but instead will be simulated to take place in a 5-stage pipelined CPU 
 * This will consist of IF,ID,EX,MEM,WB stages which will take place concurrently
 */

#include "cpu.hpp"
#include "pipeline.hpp"
#include "instructions.hpp"
#include "binary_parser.hpp"
#include "array_helpers.hpp"
#include <deque>
#include <cstdint>
#include <functional>
#include <iostream>
#include <unistd.h>

#define SET 1
#define INCREMENT 1
#define NUM_STAGES 5

// This jump table will help us easily call each stage of pipeline
std::function<void(MIPSCPU&)> stage_v_table[NUM_STAGES] =
{
     &IF_STAGE,
     &ID_STAGE,
     &EX_STAGE,
     &MEM_STAGE,
     &WB_STAGE
};

Stage incrementStage(const Stage currentStage)
{
     return static_cast<Stage>(static_cast<int>(currentStage) + 1);
}

// We will use a dequeue to simulate the concurrency of the stages. Every new instruction introduced to the queue will start in the IF stage, and move through the stages 
// until it reaches WB where it will then be removed. This means the pipeline will have at most 5 instructions at one time, which will be executed in order based
// on the way they came. The execution will be implemented by calling the function at the index of the v_table corresponding to current stage (0-4)
void clockCycle(MIPSCPU& cpu) 
{
     cpu.pipeline.push_back(Stage::IF_STAGE);
     for (auto& stage : cpu.pipeline)
     {    
          stage_v_table[static_cast<int>(stage)](cpu);
          if (stage == Stage::WB_STAGE) 
          {
               cpu.pipeline.pop_front();
          }
          stage = incrementStage(stage);
     }
}

// Simple fetch stage which reads instruction at current program counter
void IF_STAGE(MIPSCPU& cpu) 
{
     cpu.latches.IF_ID_LATCH = readContents(cpu.pc++);
}

// Decode stage will split instruction into corresponding tokens and store it in the appropriate buffer. We will use a tagged union to transfer information
// on what kind of instruction is seen through the buffer
void ID_STAGE(MIPSCPU& cpu) 
{
     const int32_t instruction = cpu.latches.IF_ID_LATCH;
     const int32_t opcode = BinaryParser::extractOpcode(instruction);
     const Instruction instructionTag = static_cast<Instruction>(opcode);

     // Switch case will fill the union with the appropriate instruction response type (depending on number and type of tokens)
     switch(opcode)
     {
          case NOP_OPCODE():
               cpu.latches.ID_EX_LATCH = ID_RESULT{
                    .type = instructionTag,            
               };
               break;        

          case ADDI_OPCODE():
               cpu.latches.ID_EX_LATCH = ID_RESULT{
                    .type = instructionTag,
                    .ADDI_instruction = BinaryParser::PARSE_ADDI(instruction, cpu)               
               };
               break;        

          case B_OPCODE():
               cpu.latches.ID_EX_LATCH = ID_RESULT{
                    .type = instructionTag,
                    .B_instruction = BinaryParser::PARSE_B(instruction, cpu)
               };
               break;          

          case BEQZ_OPCODE():
               cpu.latches.ID_EX_LATCH = ID_RESULT{
                    .type = instructionTag,
                    .BEQZ_instruction = BinaryParser::PARSE_BEQZ(instruction, cpu)
               };
               break;          

          case BGE_OPCODE():
               cpu.latches.ID_EX_LATCH = ID_RESULT{
                    .type = instructionTag,
                    .BGE_instruction = BinaryParser::PARSE_BGE(instruction, cpu)
               };
               break;          

          case BNE_OPCODE():
               cpu.latches.ID_EX_LATCH = ID_RESULT{
                    .type = instructionTag,
                    .BNE_instruction = BinaryParser::PARSE_BNE(instruction, cpu)
               };
               break;          

          case LA_OPCODE():
               cpu.latches.ID_EX_LATCH = ID_RESULT{
                    .type = instructionTag,
                    .LA_instruction = BinaryParser::PARSE_LA(instruction, cpu)
               };
               break;          

          case LB_OPCODE():
               cpu.latches.ID_EX_LATCH = ID_RESULT{
                    .type = instructionTag,
                    .LB_instruction = BinaryParser::PARSE_LB(instruction, cpu)
               };
               break;          

          case LI_OPCODE():
               cpu.latches.ID_EX_LATCH = ID_RESULT{
                    .type = instructionTag,
                    .LI_instruction = BinaryParser::PARSE_LI(instruction, cpu)
               };
               break;          

          case SUBI_OPCODE():
               cpu.latches.ID_EX_LATCH = ID_RESULT{
                    .type = instructionTag,
                    .SUBI_instruction = BinaryParser::PARSE_SUBI(instruction, cpu)
               };
               break;         

          case SYSCALL_OPCODE():
               cpu.latches.ID_EX_LATCH = ID_RESULT{
                    .type = instructionTag
               };
               break;
     }

     // Make sure to update CPU usage statistics
     cpu.cyclesPerUnit(SET, cyclesTableClosure()()[opcode].data()); // Increment cycles of each functional unit
     cpu.instructionsExecuted(INCREMENT);      // Incremenent total instructions executed
     cpu.nopCount(opcode==NOP_OPCODE());
}

// Here we will call our corresponding opcode functions that will perform the appropriate arithmetic and store in the next latch
void EX_STAGE(MIPSCPU& cpu) 
{
     switch(cpu.latches.ID_EX_LATCH.type)
     {
          case Instruction::NOP:
               cpu.latches.EX_MEM_LATCH = EX_Result{MIPS_TYPE::NType, NULL, 0};               
               break;
          case Instruction::ADDI:
               cpu.latches.EX_MEM_LATCH = ADDI(cpu.latches.ID_EX_LATCH.ADDI_instruction);               
               break;
          case Instruction::B:
               cpu.latches.EX_MEM_LATCH = B(cpu.latches.ID_EX_LATCH.B_instruction, cpu);               
               break;
          case Instruction::BEQZ:
               cpu.latches.EX_MEM_LATCH = BEQZ(cpu.latches.ID_EX_LATCH.BEQZ_instruction, cpu);               
               break;
          case Instruction::BGE:
               cpu.latches.EX_MEM_LATCH = BGE(cpu.latches.ID_EX_LATCH.BGE_instruction, cpu);               
               break;
          case Instruction::BNE:
               cpu.latches.EX_MEM_LATCH = BNE(cpu.latches.ID_EX_LATCH.BNE_instruction, cpu);               
               break;
          case Instruction::LA:
               cpu.latches.EX_MEM_LATCH = LA(cpu.latches.ID_EX_LATCH.LA_instruction, cpu);            
               break;
          case Instruction::LB:
               cpu.latches.EX_MEM_LATCH = LB(cpu.latches.ID_EX_LATCH.LB_instruction, cpu);            
               break;
          case Instruction::LI:
               cpu.latches.EX_MEM_LATCH = LI(cpu.latches.ID_EX_LATCH.LI_instruction);            
               break;
          case Instruction::SUBI:
               cpu.latches.EX_MEM_LATCH = SUBI(cpu.latches.ID_EX_LATCH.SUBI_instruction);       
               break;
          case Instruction::SYSCALL:
               cpu.latches.EX_MEM_LATCH = EX_Result{MIPS_TYPE::SysType, NULL, 0};  
               break;
     }              
}

// Here we will write to memory if necessary for our load instructions
void MEM_STAGE(MIPSCPU& cpu) 
{
     switch(cpu.latches.EX_MEM_LATCH.type)
     {
          case MIPS_TYPE::NType:
               cpu.latches.MEM_WB_LATCH = MEM_Result{NULL, 0};
               break;
          case MIPS_TYPE::RType:
               cpu.latches.MEM_WB_LATCH = MEM_Result{cpu.latches.EX_MEM_LATCH.destination, cpu.latches.EX_MEM_LATCH.result};
               break;
          case MIPS_TYPE::IType:
               writeContents(cpu.latches.EX_MEM_LATCH.destination, cpu.latches.EX_MEM_LATCH.result);
               cpu.latches.MEM_WB_LATCH = MEM_Result{NULL, 0};
               break;
          case MIPS_TYPE::JType:
               cpu.latches.MEM_WB_LATCH = MEM_Result{NULL, 0};
               break;
          case MIPS_TYPE::SysType:
               SYSCALL(cpu);
               cpu.latches.MEM_WB_LATCH = MEM_Result{NULL, 0};
               break;
     }
}

// Simple register writeback stage for R-Type Instructions
void WB_STAGE(MIPSCPU& cpu) 
{
     if (cpu.latches.MEM_WB_LATCH.destination != NULL)
     {
          *cpu.latches.MEM_WB_LATCH.destination = cpu.latches.MEM_WB_LATCH.result;
     }
}
