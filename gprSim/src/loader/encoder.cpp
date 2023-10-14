/* 
 * @brief encoder does something cool
 *
 * more
 * more
 * more
 */

#include "encoder.hpp"
#include "instructions.hpp"
#include <iostream>

constexpr const int NO_OFFSET           = 0;
constexpr const int REGISTER_LENGTH     = 5;
constexpr const int IMMEDIATE_LENGTH    = 16;
constexpr const int OFFSET_LENGTH       = 21;
constexpr const int ALL_ZEROES          = 0;
constexpr const int BYTES_IN_ADDRESS    = 4; // MIPS System has 4 bytes per instruction

namespace // Offset Helpers
{
    OffsetInfo getOffsetInfo(const std::string& token)
    {
        std::string integer_portion_str = "";
        int register_portion_index;
        for (size_t i = 0; i < token.size(); i++) {
            if (std::isdigit(token[i])) {
                integer_portion_str += token[i];
            }
            else {
                register_portion_index = i;
                break;
            }
        }
        return OffsetInfo{integer_portion_str, register_portion_index};
    }

    OffsetInstruction splitOffsetInstruction(const std::string& token)
    {
        int integer_portion = NO_OFFSET;
        const OffsetInfo offsetInfo = getOffsetInfo(token);
        if (offsetInfo.integer_portion_str.size() > 0) {
            integer_portion = std::stoi(offsetInfo.integer_portion_str);
        }
        const std::string register_portion = token.substr(offsetInfo.register_portion_index+1, token.size()-2); // ($t3) cut off parantheses
        return OffsetInstruction{integer_portion, Encoder::encodeRegister(register_portion)};
    }
}

int32_t Encoder::encodeOpCode(const std::string& token)
{
    return OpcodeTable::getOpcode(token);
}

int32_t Encoder::encodeRegister(const std::string& token)
{
    return RegisterTable::getRegister(token);
}

int32_t Encoder::encodeImmediate(const std::string& token)
{
    return std::stoi(token);
}

int32_t Encoder::encodeOffset(const std::string& token)
{
    int32_t instruction = ALL_ZEROES;
    const OffsetInstruction splitInstruction = splitOffsetInstruction(token);
    instruction |= (splitInstruction.register_portion << OFFSET_LENGTH-REGISTER_LENGTH);
    instruction |= (splitInstruction.integer_portion  << OFFSET_LENGTH-OFFSET_LENGTH); 
    return instruction;
}

int32_t Encoder::encodeLabel(const std::string& token, Memory& memory, int32_t* LOCCTR)
{
    const char* label_address           = (char*)memory.symbol_table[token];
    const char* memory_address          = (char*)&memory;
    const char* LOCCTR_address          = (char*)LOCCTR;
    const int32_t offsetFromPC          = static_cast<int32_t>(label_address-LOCCTR_address); 
    const int32_t offsetFromPCInBytes   = offsetFromPC / BYTES_IN_ADDRESS;
    return offsetFromPCInBytes;
}