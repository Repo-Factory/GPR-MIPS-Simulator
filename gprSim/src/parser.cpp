/* 
 * @brief parser does something cool
 *
 * Instruction Set 
 * ----------------------
 * ADDI Rdest, Rsrc1, Imm
 * B label
 * BEQZ Rsrc1, label
 * BGE Rsrc1, Rsrc2, label
 * BNE Rsrc1, Rsrc2, label
 * LA Rdest, label
 * LB Rdest, offset(Rsrc1)
 * LI Rdest, Imm
 * SUBI Rdest, Rsrc1, Imm
 * SYSCALL
 */

#include "parser.hpp"
#include "stdio.h"
#include "stdlib.h"
#include "bit_operations.hpp"
#include "file_handling_helpers.hpp"

constexpr const int INSTRUCTION_LENGTH_BITS = 32;
constexpr const int OPCODE_LENGTH_BITS = 6;
constexpr const int REGISTER_LENGTH = 5;
constexpr const int IMMEDIATE_LENGTH = 16;
constexpr const int ALL_ZEROES = 0;

// OPCODE 6 bits | $Register 5 bits | Label - 32-USED bits | Imm - 16 bits

constexpr const char*   ANSWER_SYMBOL                           =   "ANS";
constexpr const char*   ADDI_INSTRUCTION                        =   "ADDI";
constexpr const char*   B_INSTRUCTION                           =   "B";
constexpr const char*   BEQZ_INSTRUCTION                        =   "BEQZ";
constexpr const char*   BGE_INSTRUCTION                         =   "MUL";
constexpr const char*   BNE_INSTRUCTION                         =   "BNE";
constexpr const char*   LA_INSTRUCTION                          =   "LA";
constexpr const char*   LB_INSTRUCTION                          =   "LB";
constexpr const char*   LI_INSTRUCTION                          =   "LI";
constexpr const char*   SUBI_INSTRUCTION                        =   "SUBI";
constexpr const char*   SYSCALL_INSTRUCTION                     =   "SYSCALL";

constexpr const int32_t ANSWER_BIT_STREAM                       =   computeBitStream(ANSWER_SYMBOL);        // Defined in helpers/bit_operations.hpp
constexpr const int32_t ADDI_INSTRUCTION_BIT_STREAM             =   computeBitStream(ADDI_INSTRUCTION);
constexpr const int32_t B_INSTRUCTION_BIT_STREAM                =   computeBitStream(B_INSTRUCTION);
constexpr const int32_t BEQZ_INSTRUCTION_BIT_STREAM             =   computeBitStream(BEQZ_INSTRUCTION);
constexpr const int32_t BGE_INSTRUCTION_BIT_STREAM              =   computeBitStream(BGE_INSTRUCTION);
constexpr const int32_t BNE_INSTRUCTION_BIT_STREAM              =   computeBitStream(BNE_INSTRUCTION);
constexpr const int32_t LA_INSTRUCTION_BIT_STREAM               =   computeBitStream(LA_INSTRUCTION);
constexpr const int32_t LB_INSTRUCTION_BIT_STREAM               =   computeBitStream(LB_INSTRUCTION);
constexpr const int32_t LI_INSTRUCTION_BIT_STREAM               =   computeBitStream(LI_INSTRUCTION);
constexpr const int32_t SUBI_INSTRUCTION_BIT_STREAM             =   computeBitStream(SUBI_INSTRUCTION);
// constexpr const int32_t SYSCALL_INSTRUCTION_BIT_STREAM          =   computeBitStream(SYSCALL_INSTRUCTION);


int32_t Parser::parseInstruction(const std::string& next_instruction)
{
    int current_bit = INSTRUCTION_LENGTH_BITS; // If we have 32 bits to fill, we start from the 32nd leftmost bit
    iterateTokens(next_instruction, [&](const std::string& token) {
             
    });
}

bool Parser::isInstruction(const std::string& instruction)
{
    return countWords(instruction) > 1;    
}

