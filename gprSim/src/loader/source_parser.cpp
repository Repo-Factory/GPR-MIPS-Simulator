/* 
 * @brief Parse Source File Based on Instruction Specs
 *
 *          Instruction Set 
 *      ----------------------
 *      ADDI Rdest, Rsrc1, Imm
 *      B label
 *      BEQZ Rsrc1, label
 *      BGE Rsrc1, Rsrc2, label
 *      BNE Rsrc1, Rsrc2, label
 *      LA Rdest, label
 *      LB Rdest, offset(Rsrc1)
 *      LI Rdest, Imm
 *      SUBI Rdest, Rsrc1, Imm
 *      SYSCALL
 */

#include "stdio.h"
#include "stdlib.h"
#include "source_parser.hpp"
#include "encoder.hpp"
#include "file_handling_helpers.hpp"
#include "instructions.hpp"
#include <iostream>
#include <cmath>
#include <set>
#include <algorithm>

// Lengths in bits
constexpr const int INSTRUCTION_LENGTH                          = 32;
constexpr const int OPCODE_LENGTH                               = 6;
constexpr const int REGISTER_LENGTH                             = 5;
constexpr const int IMMEDIATE_LENGTH                            = 16;
constexpr const int OFFSET_LENGTH                               = 21;
constexpr const int ALL_ZEROES                                  = 0;

namespace
{
    constexpr const char REGISTER_IDENTIFIER = '$';
    constexpr const char OFFSET_IDENTIFIER =   ')';

    bool is_opcode(const std::string& token)
    {
        return OpcodeTable::searchTable(token);
    }

    bool is_register(const std::string& token)
    {
        return token.front()==REGISTER_IDENTIFIER;
    }

    bool is_offset(const std::string& token)
    {
        return token.back()==OFFSET_IDENTIFIER;
    }

    bool is_immediate(const std::string& str) 
    {
        if (str.empty()) return false;
        for (char c : str) {
            if (!std::isdigit(c)) {
                return false;
            }
        }
        return true;
    }
}

namespace
{
    // Pass in memory for the symbol table
    BitStream getBitStreamFromToken(const int current_bit, const std::string& token, Memory& memory, int32_t* LOCCTR)
    {
        if (is_opcode(token)) 
            return BitStream{OPCODE_LENGTH, Encoder::encodeOpCode(token)};
        if (is_register(token)) 
            return BitStream{REGISTER_LENGTH, Encoder::encodeRegister(token.substr(0,token.size()-1))};
        if (is_offset(token)) 
            return BitStream{OFFSET_LENGTH, Encoder::encodeOffset(token)};
        if (is_immediate(token)) 
            return BitStream{current_bit, Encoder::encodeImmediate(token)};
        else 
            return BitStream{current_bit, (Encoder::encodeLabel(token, memory, LOCCTR)) & ((1<<current_bit)-1)};
    }
}

int32_t Parser::parseInstruction(const std::string& next_instruction, Memory& memory, int32_t* LOCCTR)
{
    int current_bit = INSTRUCTION_LENGTH;      // If we have 32 bits to fill, we start from the 32nd leftmost bit
    int32_t instruction = ALL_ZEROES;
    iterateTokens(next_instruction, [&](const std::string& token) {
        const BitStream bit_stream = getBitStreamFromToken(current_bit, token, memory, LOCCTR);
        instruction |= (bit_stream.stream << (current_bit -= bit_stream.size));
    });
    return instruction;
}

// Checks if label or instruction
bool Parser::isInstruction(const std::string& instruction)
{
    const std::string firstWord = getFirstWordOfLine(instruction);
    return OpcodeTable::searchTable(firstWord);
}

bool Parser::isBranchInstruction(const std::string& instruction)
{
    const std::string firstWord = getFirstWordOfLine(instruction);
    std::set<std::string> branchInstructions {"b","beqz","bge","bne"}; 
    return branchInstructions.count((firstWord));
}