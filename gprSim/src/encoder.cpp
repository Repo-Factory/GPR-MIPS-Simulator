/* 
 * @brief encoder does something cool
 *
 * more
 * more
 * more
 */

#include "encoder.hpp"
#include "instructions.hpp"

constexpr const int NO_OFFSET           = 0;
constexpr const int REGISTER_LENGTH     = 5;
constexpr const int IMMEDIATE_LENGTH    = 16;
constexpr const int ALL_ZEROES          = 0;


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
        OffsetInfo offsetInfo = getOffsetInfo(token);
        if (offsetInfo.integer_portion_str.size() > 0) {
            integer_portion = std::stoi(offsetInfo.integer_portion_str);
        }
        std::string register_portion = token.substr(offsetInfo.register_portion_index+1, token.size()-1); // ($t3) cut off parantheses
        return OffsetInstruction{integer_portion, Encoder::encodeRegister(register_portion)};
    }
}
constexpr const int IGNORE_FIRST_CHAR = 1;
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
    instruction |= (splitInstruction.register_portion << REGISTER_LENGTH);
    instruction |= (splitInstruction.integer_portion  << IMMEDIATE_LENGTH); 
    return instruction;
}

int32_t Encoder::encodeLabel(const std::string& token, const Memory& memory)
{
    return *memory.symbol_table.at(token);
}

