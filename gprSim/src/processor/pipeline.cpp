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
     cpu.pipeline.push_front(Stage::IF_STAGE);
     for (auto& stage : cpu.pipeline)
     {    
          stage_v_table[static_cast<int>(stage)](cpu);
          if (stage == Stage::WB_STAGE) 
          {
               cpu.pipeline.pop_back();
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
     // const int32_t instruction = cpu.latches.IF_ID_LATCH;
     // const int32_t opcode = BinaryParser::extractOpcode(instruction);
     // const Instruction instructionTag = static_cast<Instruction>(opcode);

     // switch(opcode)
     // {
     //      case ADDI_OPCODE():
     //           cpu.latches.ID_EX_LATCH = InstructionType{
     //                .type = instructionTag,
     //                .ADDI_instruction = BinaryParser::PARSE_ADDI(instruction, cpu)               
     //           };        

     //      case B_OPCODE():
     //           cpu.latches.ID_EX_LATCH = InstructionType{
     //                .type = instructionTag,
     //                .B_instruction = BinaryParser::PARSE_B(instruction, cpu)
     //           };          

     //      case BEQZ_OPCODE():
     //           cpu.latches.ID_EX_LATCH = InstructionType{
     //                .type = instructionTag,
     //                .BEQZ_instruction = BinaryParser::PARSE_BEQZ(instruction, cpu)
     //           };          

     //      case BGE_OPCODE():
     //           cpu.latches.ID_EX_LATCH = InstructionType{
     //                .type = instructionTag,
     //                .BGE_instruction = BinaryParser::PARSE_BGE(instruction, cpu)
     //           };          

     //      case BNE_OPCODE():
     //           cpu.latches.ID_EX_LATCH = InstructionType{
     //                .type = instructionTag,
     //                .BNE_instruction = BinaryParser::PARSE_BNE(instruction, cpu)
     //           };          

     //      case LA_OPCODE():
     //           cpu.latches.ID_EX_LATCH = InstructionType{
     //                .type = instructionTag,
     //                .LA_instruction = BinaryParser::PARSE_LA(instruction, cpu)
     //           };          

     //      case LB_OPCODE():
     //           cpu.latches.ID_EX_LATCH = InstructionType{
     //                .type = instructionTag,
     //                .LB_instruction = BinaryParser::PARSE_LB(instruction, cpu)
     //           };          

     //      case LI_OPCODE():
     //           cpu.latches.ID_EX_LATCH = InstructionType{
     //                .type = instructionTag,
     //                .LI_instruction = BinaryParser::PARSE_LI(instruction, cpu)
     //           };          

     //      case SUBI_OPCODE():
     //           cpu.latches.ID_EX_LATCH = InstructionType{
     //                .type = instructionTag,
     //                .SUBI_instruction = BinaryParser::PARSE_SUBI(instruction, cpu)
     //           };          
     // }
}

void EX_STAGE(MIPSCPU& cpu) 
{
     // switch(cpu.latches.ID_EX_LATCH.type)
     // {
     //      case Instruction::ADDI:
     //           ADDI(cpu.latches.ID_EX_LATCH.ADDI_instruction);               

     //      case Instruction::B:
     //           B(cpu.latches.ID_EX_LATCH.B_instruction, cpu);               

     //      case Instruction::BEQZ:
     //           BEQZ(cpu.latches.ID_EX_LATCH.BEQZ_instruction, cpu);               

     //      case Instruction::BGE:
     //           BGE(cpu.latches.ID_EX_LATCH.BGE_instruction, cpu);               

     //      case Instruction::BNE:
     //           BNE(cpu.latches.ID_EX_LATCH.BNE_instruction, cpu);               

     //      case Instruction::LA:
     //           LA(cpu.latches.ID_EX_LATCH.LA_instruction, cpu);            

     //      case Instruction::LB:
     //           LB(cpu.latches.ID_EX_LATCH.LB_instruction, cpu);            

     //      case Instruction::LI:
     //           LI(cpu.latches.ID_EX_LATCH.LI_instruction);            

     //      case Instruction::SUBI:
     //           SUBI(cpu.latches.ID_EX_LATCH.SUBI_instruction);             
     // }
}

void MEM_STAGE(MIPSCPU& cpu) 
{

}

void WB_STAGE(MIPSCPU& cpu) 
{

}



//    cpu.cyclesPerUnit(INCREMENT, cyclesTableClosure()()[opcode].data());
