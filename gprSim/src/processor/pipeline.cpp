/* 
 * @brief 
 *
 * 
 */
#include "cpu.hpp"
#include "pipeline.hpp"
#include "instructions.hpp"
#include "binary_parser.hpp"
#include <deque>
#include <cstdint>
#include <functional>
#include <iostream>
#include <unistd.h>

#define INCREMENT 1
#define NUM_STAGES 5

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

void executeInstruction(MIPSCPU& cpu) 
{
     cpu.pipeline.push_back(Stage::IF_STAGE);
     for (auto& stage : cpu.pipeline)
     {    
          stage_v_table[static_cast<int>(stage)](cpu);
          // std::cout << (int)(stage); std::cout.flush();
          // sleep(1);
          if (stage == Stage::WB_STAGE) 
          {
               cpu.pipeline.pop_front();
          }
          stage = incrementStage(stage);
     }
}

void IF_STAGE(MIPSCPU& cpu) 
{
     cpu.latches.IF_ID_LATCH = readContents(cpu.pc++);
}

void ID_STAGE(MIPSCPU& cpu) 
{
     const int32_t instruction = cpu.latches.IF_ID_LATCH;
     const int32_t opcode = BinaryParser::extractOpcode(instruction);
     const Instruction instructionTag = static_cast<Instruction>(opcode);

     switch(opcode)
     {
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
}

void EX_STAGE(MIPSCPU& cpu) 
{
     switch(cpu.latches.ID_EX_LATCH.type)
     {
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

void MEM_STAGE(MIPSCPU& cpu) 
{
     switch(cpu.latches.EX_MEM_LATCH.type)
     {
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

void WB_STAGE(MIPSCPU& cpu) 
{
     if (cpu.latches.EX_MEM_LATCH.destination != NULL)
     {
          *cpu.latches.EX_MEM_LATCH.destination = cpu.latches.EX_MEM_LATCH.result;
     }
}
